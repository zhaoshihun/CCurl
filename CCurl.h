/*
* CCurl.h
*
*  Created on: 2012-3-21
*      Author: zsh
*/
/*Encapsulate curl http://curl.haxx.se/libcurl/ */
#ifndef CCURL_H_
#define CCURL_H_

typedef void CURL;
#include<string>
struct curl_slist ;
namespace CCCurl
{
	class RequestMethod
	{
	public:
		static const std::string GET;
		static const std::string POST;
	};
	class CCurl
	{
	public:
		CCurl();
		~CCurl();
		void Connection(const std::string &url);
		void AddHeader(const char *header);
		bool Send(const std::string &method,const char *buffer,unsigned int bufferLength);
		static void ReceiveFunc(void *ptr,size_t size,size_t nmemb,void *userdata);//receive data  callback function. you can handle recv data in this function.
		static void RecvHeaderFunc(void *ptr,size_t size,size_t nmemb,void *userdata);

	private:
		bool GetMethod(const char *buffer,unsigned int bufferLength);
		bool PostMethod(const char *buffer,unsigned int bufferLength);
		std::string url;
		CURL* easy_handle;
		curl_slist *headers;
		bool  hasHeader;
	};
}
#endif /* CCURL_H_ */
