#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"

#include "ofURLFileLoader.h"
#include "ofAppRunner.h"
#include "ofUtils.h"

using namespace Poco::Net;

using namespace Poco;

#include "ofConstants.h"

static bool factoryLoaded = false;
int	ofHttpRequest::nextID = 0;
ofEvent<ofHttpResponse> & ofURLResponseEvent(){
	static ofEvent<ofHttpResponse> * event = new ofEvent<ofHttpResponse>;
	return *event;
}

ofEvent<ofHttpResponse> & ofURLProgressEvent(){
	static ofEvent<ofHttpResponse> * event = new ofEvent<ofHttpResponse>;
	return *event;
}

ofURLFileLoader::ofURLFileLoader() {
	if(!factoryLoaded){
		try {
			HTTPStreamFactory::registerFactory();
			HTTPSStreamFactory::registerFactory();
			SharedPtr<PrivateKeyPassphraseHandler> pConsoleHandler = new KeyConsoleHandler(false);
			SharedPtr<InvalidCertificateHandler> pInvalidCertHandler = new ConsoleCertificateHandler(true);
			Context::Ptr pContext = new Context(Context::CLIENT_USE, "", Context::VERIFY_NONE);
			SSLManager::instance().initializeClient(pConsoleHandler, pInvalidCertHandler, pContext);
			factoryLoaded = true;
		}
		catch (Poco::SystemException & PS) {
			ofLog(OF_LOG_ERROR, "Got exception in url ofURLFileloader");
		}
	}
}

ofHttpResponse ofURLFileLoader::get(string url) {
    ofHttpRequest request(url,url);
    return handleRequest(request);
}


int ofURLFileLoader::getAsync(string url, string name){
	if(name=="") name=url;
	ofHttpRequest request(url,name);
	lock();
	requests.push_back(request);
	unlock();
	start();
	return request.getID();
}


ofHttpResponse ofURLFileLoader::saveTo(string url, string path){
    ofHttpRequest request(url,path,true);
    return handleRequest(request);
}

int ofURLFileLoader::saveAsync(string url, string path){
	ofHttpRequest request(url,path,true);
	lock();
	requests.push_back(request);
	unlock();
	start();
	return request.getID();
}

void ofURLFileLoader::remove(int id){
	Poco::ScopedLock<ofMutex> lock(mutex);
	for(int i=0;i<(int)requests.size();i++){
		if(requests[i].getID()==id){
			requests.erase(requests.begin()+i);
			return;
		}
	}
	ofLogError() << "trying to remove request " <<  id << " not found";
}

void ofURLFileLoader::clear(){
	Poco::ScopedLock<ofMutex> lock(mutex);
	requests.clear();
	while(!responses.empty()) responses.pop();
}

void ofURLFileLoader::start() {
     if (isThreadRunning() == false){
		ofAddListener(ofEvents().update,this,&ofURLFileLoader::update);
        startThread(true, false);   // blocking, verbose
    }else{
    	ofLog(OF_LOG_VERBOSE,"signaling new request condition");
    	condition.signal();
    }
}

void ofURLFileLoader::stop() {
    stopThread();
}

void ofURLFileLoader::threadedFunction() {
	lock();
	while( isThreadRunning() == true ){
    	ofLog(OF_LOG_VERBOSE,"starting thread loop ");
		if(requests.size()>0){
	    	ofLog(OF_LOG_VERBOSE,"querying request " + requests.front().name);
			ofHttpRequest request(requests.front());
			unlock();

			ofHttpResponse response(handleRequest(request));

			lock();
			if(response.status!=-1){
				// double-check that the request hasn't been removed from the queue
				if( (requests.size()==0) || (requests.front().getID()!=request.getID()) ){
					// this request has been removed from the queue
					ofLog(OF_LOG_VERBOSE,"request " + requests.front().name + " is missing from the queue, must have been removed/cancelled" );
				}
				else{
					ofLog(OF_LOG_VERBOSE,"got response to request " + requests.front().name + " status "+ofToString(response.status) );
					responses.push(response);
					requests.pop_front();
				}
			}else{
				responses.push(response);
		    	ofLog(OF_LOG_VERBOSE,"failed getting request " + requests.front().name);
			}
		}else{
			ofLog(OF_LOG_VERBOSE,"stopping on no requests condition");
			condition.wait(mutex);
		}
	}
}

ofHttpResponse ofURLFileLoader::handleRequest(ofHttpRequest request) {
	try {
		URI uri(request.url);
		std::string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";

		HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
		HTTPResponse res;
		ofPtr<HTTPSession> session;
		istream * rs;
		if(uri.getScheme()=="https"){
			 //const Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", "rootcert.pem" ) );
			HTTPSClientSession * httpsSession = new HTTPSClientSession(uri.getHost(), uri.getPort());//,context);
			httpsSession->setTimeout(Poco::Timespan(20,0));
			httpsSession->sendRequest(req);
			rs = &httpsSession->receiveResponse(res);
			session = ofPtr<HTTPSession>(httpsSession);
		}else{
			HTTPClientSession * httpSession = new HTTPClientSession(uri.getHost(), uri.getPort());
			httpSession->setTimeout(Poco::Timespan(20,0));
			httpSession->sendRequest(req);
			rs = &httpSession->receiveResponse(res);
			session = ofPtr<HTTPSession>(httpSession);
		}
        
        downloading = ofHttpResponse(request,res.getStatus(),res.getReason(),res.getContentLength());
        prevProgress = 0;
        downloading.downloading = true;
		if(!request.saveTo){
            downloading.data.set(*rs);
            downloading.downloading = false;
			return downloading;//ofHttpResponse(request,*rs,res.getStatus(),res.getReason(),res.getContentLength());
		}else{
			ofFile saveTo(request.name,ofFile::WriteOnly,true);
			char aux_buffer[1024];
			rs->read(aux_buffer, 1024);
			std::streamsize n = rs->gcount();
            downloading.downloaded += n;
			while (downloading.downloaded < downloading.size){
				// we resize to size+1 initialized to 0 to have a 0 at the end for strings
				if(n>0) saveTo.write(aux_buffer,n);
				if (rs->good()){
					rs->read(aux_buffer, 1024);
					n = rs->gcount();
                    downloading.downloaded += n;
				}else{
                    n = 0;
                }
			}
            downloading.downloading = false;
			return downloading;
		}

	} catch (const Exception& exc) {
        ofLog(OF_LOG_ERROR, "ofURLFileLoader " + exc.displayText());

        return ofHttpResponse(request,-1,exc.displayText(),0);

    } catch (...) {
    	return ofHttpResponse(request,-1,"ofURLFileLoader fatal error, couldn't catch Exception",0);
    }

	return ofHttpResponse(request,-1,"ofURLFileLoader fatal error, couldn't catch Exception",0);
	
}	

void ofURLFileLoader::update(ofEventArgs & args){
	lock();
	while(!responses.empty()){
		ofHttpResponse response(responses.front());
		ofLog(OF_LOG_VERBOSE,"ofURLLoader::update: new response " +response.request.name);
		responses.pop();
		unlock();
        ofNotifyEvent(ofURLProgressEvent(), response, this);
		ofNotifyEvent(ofURLResponseEvent(), response , this);
		lock();
	}
	unlock();
    if(downloading.downloading){
        int currentProgress = downloading.getDownloaded();
        if(prevProgress!=currentProgress){
            ofNotifyEvent(ofURLProgressEvent(), downloading, this);
            prevProgress = currentProgress;
        }
    }
}

static ofURLFileLoader & getFileLoader(){
	static ofURLFileLoader * fileLoader = new ofURLFileLoader;
	return *fileLoader;
}

ofHttpResponse ofLoadURL(string url){
	return getFileLoader().get(url);
}

int ofLoadURLAsync(string url, string name){
	return getFileLoader().getAsync(url,name);
}

ofHttpResponse ofSaveURLTo(string url, string path){
	return getFileLoader().saveTo(url,path);
}

int ofSaveURLAsync(string url, string path){
	return getFileLoader().saveAsync(url,path);
}

void ofRemoveURLRequest(int id){
	getFileLoader().remove(id);
}

void ofRemoveAllURLRequests(){
	getFileLoader().clear();
}
