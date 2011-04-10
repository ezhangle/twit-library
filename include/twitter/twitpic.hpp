///////////////////////////////////////////////////////////////////////////////
// twitpic.hpp header file
// twit-library : http://code.google.com/p/twit-library/
//
// TwitPicを使うためだけに作られたクラスだよー
// Twitterクラスで派生することによってTwitPicを手軽に使える

#ifndef TWITPIC_HPP
#define TWITPIC_HPP

#include "../oauth.hpp"

namespace oauth{
	namespace client{
		template<class Protocol_Type>
		class twitpic : public virtual oauth::client::client_base<Protocol_Type>
		{
		public:
			twitpic() : twitpic_host_("api.twitpic.com"){}
			~twitpic(){}
			
			void set_twitpic_key(const string_type& key){twitpic_key_ = key;}

			std::string twitpic_upload(const string_type& format,const string_type& file_path,const string_type& file_name,const string_type& message)
			{
				//通信用クラス生成
				client_  = new Protocol_Type(io_service_);

				const string_type method = "POST";
				const string_type sig_url = "https://api.twitter.com/1/account/verify_credentials.json";
				const string_type boundary = "---------------------------"+util::make_nonce(10);
				const string_type path = "/2/upload."+format;
				
				//ヘッダーの作成
				param_type head;
				head["X-Verify-Credentials-Authorization"] = make_signature("GET",sig_url);
				head["X-Auth-Service-Provider"] = sig_url;
				head["Content-Type"] = "multipart/form-data; boundary=" + boundary;
				
				//ボディを羅列
				std::vector<oauth::content::multipart_form_data> datas;
				{
					oauth::content::multipart_form_data data(oauth::content::content_type::Data,"key",twitpic_key_);
					datas.push_back(data);
				}
				{
					oauth::content::multipart_form_data data(oauth::content::content_type::Data,"message",message);
					datas.push_back(data);
				}
				{
					oauth::content::multipart_form_data data(oauth::content::content_type::Image,"media",file_path,file_name);
					datas.push_back(data);
				}
				
				const auto body = make_body(datas,boundary);
				
				//通信
				boost::asio::streambuf buffer;
				client_->write(
					twitpic_host_,
					&make_request(buffer,method,twitpic_host_,path,head,body)
					);

				string_type res;
				client_->read(res);
				delete client_;
				client_=NULL;

				return res;

				//リクエストヘッダの作成
				/*request_header head;
				head["X-Verify-Credentials-Authorization"] = make_authorize(key,url,"GET");
				head["X-Auth-Service-Provider"] = url;
				head["Content-Type"] = "multipart/form-data; boundary=" + boundary;

				std::stringstream body;
				{
					body << "--" << boundary << "\r\n";
					body << "Content-Disposition: form-data; name=\"media\"; filename=\"" << file_name << "\";" << "\r\n";
					body << "Content-Type: " << util::image::chk_type(file_name) << "\r\n";
					body << "\r\n";
					body << util::image::loader(file_path+file_name) << "\r\n";
			
					body << "--" << boundary << "\r\n";
					body << "Content-Disposition: form-data; name=\"key\"" << "\r\n";
					body << "\r\n";
					body << api_key << "\r\n";

					body << "--" << boundary << "\r\n";
					body << "Content-Disposition: form-data; name=\"message\"" << "\r\n";
					body << "\r\n";
					body << message << "\r\n";
			
					body << "--" << boundary << "--"<< "\r\n\r\n";
				}

				http_.request("POST",twitpic_url_,"/2/upload.xml",head,body.str());
				return http_.response();*/
			}

			std::string twitpic_upload(const string_type& format,const std::string& api_key,const std::string& file_path,const std::string& file_name,const std::string& message)
			{
				set_twitpic_key(api_key);
				twitpic_upload(file_path,file_name,message,format);
			}

			std::string twitpic_upload_XML(const string_type& file_path,const string_type& file_name,const string_type& message)
			{
				return twitpic_upload("xml",file_path,file_name,message);
			}
			std::string twitpic_upload_XML(const string_type& api_key,const string_type& file_path,const string_type& file_name,const string_type& message)
			{
				set_twitpic_key(api_key);
				return twitpic_upload("xml",file_path,file_name,message);
			}
			
			std::string twitpic_upload_JSON(const string_type& file_path,const string_type& file_name,const string_type& message)
			{
				return twitpic_upload("json",file_path,file_name,message);
			}
			std::string twitpic_upload_JSON(const string_type& api_key,const string_type& file_path,const string_type& file_name,const string_type& message)
			{
				set_twitpic_key(api_key);
				return twitpic_upload("json",file_path,file_name,message);
			}

		private:
			const string_type twitpic_host_;
			string_type twitpic_key_;
		};
	}//End client namespace
}//End oauth namespace
#endif
