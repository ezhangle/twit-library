///////////////////////////////////////////////////////////////////////////////
// api.hpp header file
// twit-library : http://code.google.com/p/twit-library/
//
// OAuthAPIの雛形クラスだよー
// こいつを拡張すれば大抵のAPIは動いちゃう
// method_ : GET or POST or DELETE ...
// protocol_ : http or https
// host_ : api.foo.com
// path_ : /foo.xml
// content_type_ : application/x-www-form-urlencoded or multipart/form-data or application/x.atom+xml
// あとの使い方は./base.hpp参照

#ifndef OAUTH_API_BASE_HPP
#define OAUTH_API_BASE_HPP

#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace oauth{
	namespace api{
		//API実装基準クラス
		class base{
		public:
			base(const std::string method,const std::string protocol,const std::string host,const std::string path,const std::string content_type="application/x-www-form-urlencoded")
				: method_(method), protocol_(protocol), host_(host), path_(path), content_type_(content_type){}
			std::string method() const{return method_;}
			std::string protocol() const{return protocol_;}
			std::string host() const{return host_;}
			std::string path() const{return path_;}
			std::string content_type() const{return content_type_;}
			virtual param_type sort() const{return param_type();}
			virtual std::vector<oauth::content::multipart_form_data> data() const{return std::vector<oauth::content::multipart_form_data>();}
			virtual boost::property_tree::ptree xml_ptree() const{return boost::property_tree::ptree();}
			virtual param_type header() const{return std::map<std::string,std::string>();}
		private:
			const std::string method_;
			const std::string protocol_;
			const std::string host_;
			const std::string path_;
			const std::string content_type_;
		};
	}
}

#endif
