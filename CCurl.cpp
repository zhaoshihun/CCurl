/*
 * CCurl.cpp
 *
 *  Created on: 2012-3-21
 *      Author: zsh
 */
#include "CCurl.h"
#include "../curl/curl.h"
namespace CCCurl
{
	const std::string RequestMethod::GET = "Get";
	const std::string RequestMethod::POST = "Post";
	CCurl::CCurl()
	{
		easy_handle = curl_easy_init();
		headers=NULL;
		hasHeader = false;
		url = "";
	}
	CCurl::~CCurl()
	{
		curl_easy_cleanup(easy_handle);
	}
	void CCurl::Connection(const std::string &url)
	{
		this->url = url;
	}
	void CCurl::AddHeader(const char *header)
	{
		hasHeader =true;
		headers=curl_slist_append(headers,header);
	}
	bool CCurl::Send(const std::string &url,const char *buffer,unsigned int bufferLength)
	{
		if(buffer ==NULL || bufferLength ==0 || url == "")
			return false;
		if(url.compare(RequestMethod::GET)==0)
		{
			return GetMethod(buffer,bufferLength);
		}
		else if(url.compare(RequestMethod::POST)==0)
		{
			return PostMethod(buffer,bufferLength);
		}
	}
	bool CCurl::GetMethod(const char *buffer,unsigned int bufferLength)
	{
		char *encodedURL = curl_easy_escape(easy_handle, buffer, strlen(buffer));
		url.append(encodedURL);
		struct curl_slist *header=NULL;
		header= curl_slist_append(header,url.c_str());
		curl_easy_setopt(easy_handle, CURLOPT_VERBOSE,1L);/*open comment when debug mode.*/
		curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
		curl_easy_setopt(easy_handle,CURLOPT_HTTPHEADER,header);
		CURLcode code = curl_easy_perform(easy_handle);//
		curl_slist_free_all(header);
		curl_free(encodedURL);

		return code == CURLE_OK;
	}
	bool CCurl::PostMethod(const char *buffer,unsigned int bufferLength)
	{

		if(hasHeader)
			curl_easy_setopt(easy_handle,CURLOPT_HTTPHEADER,headers);
		char *encodedURL = curl_easy_escape(easy_handle, buffer, bufferLength);

		curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, RecvHeaderFunc);
		//curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA,&head);

		curl_easy_setopt(easy_handle,CURLOPT_POSTFIELDS,encodedURL);
		curl_easy_setopt(easy_handle,CURLOPT_POSTFIELDSIZE,strlen(encodedURL));
		curl_easy_setopt(easy_handle,CURLOPT_WRITEFUNCTION,(void*)ReceiveFunc);//receive callback function
		//curl_easy_setopt(easy_handle,CURLOPT_WRITEDATA,recv);
		curl_easy_setopt(easy_handle,CURLOPT_POST,1);
		curl_easy_setopt(easy_handle,CURLOPT_VERBOSE,1); /* open comment when debug mode.*/

		CURLcode code = curl_easy_perform(easy_handle);
		curl_slist_free_all(headers); 
		curl_free(encodedURL);
		return code == CURLE_OK;
	}
	void CCurl::ReceiveFunc(void *ptr,size_t size,size_t nmemb,void *userdata)
	{

	}
	void CCurl::RecvHeaderFunc(void *ptr,size_t size,size_t nmemb,void *userdata)
	{

	}
}
