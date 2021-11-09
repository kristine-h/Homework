#include <iostream>
#include <fstream>
#include <regex>

class Utility {
public:
        static bool IsNumber(const std::string& data);
        static bool IsInteger(const std::string& data);
        static bool IsFloat(const std::string& data);
        static bool IsBoolean(const std::string& data);
        static bool IsEmail(const std::string& data);
        static bool IsDomainNameValid(const std::string& domain_name);
        static bool IsURL(const std::string& data);
private:
	      static bool EmailContainsAtSymbol(const std::string& data);
        static void SeparateEmail(const std::string& data, std::string& email_prefix, std::string& email_domain_name, std::string& email_top_level_domain);
        static bool IsEmailPrefixValid(const std::string& prefix);
        static bool IsTopLevelDomainValid(const std::string& top_level_domain);
        static bool DoesTopLevelDomainAlreadyExist(const std::string& domain);
        static bool IsEmailUnique(const std::string& data);
};

bool Utility::IsInteger(const std::string& data) {
        if ((data[0] == '-') || ((data[0] >= '0') && (data[0] <= '9'))) {
                for (int i = 1; i < data.size(); ++i) {
                        if ((data[i] >= '0') && (data[i] <= '9')) {
                                continue;
                        } else { return false; }
                }
	}
        return true;
}

bool Utility::IsFloat(const std::string& data) {
        int floating_point_count{0};
        if ((data[0] == '-') || ((data[0] >= '0') && (data[0] <= '9') && (data[data.size() - 1] >= '0') && (data[data.size() - 1] <= '9'))) {
                for (int i = 1; i < data.size(); ++i) {
                        if ((data[i] >= '0') && (data[i] <= '9')) { continue; }
                        if (data[i] == '.') { ++floating_point_count; }
                }
        }
        if (floating_point_count == 1) { return true; }
        return false;
}

bool Utility::IsNumber(const std::string& data) {
        if (IsInteger(data) || IsFloat(data)) { return true; }
        return false;
}


bool Utility::IsBoolean(const std::string& data) {
        if ((data == "true") || (data == "false")) {
                return true;
        }
        return false;
}

bool Utility::EmailContainsAtSymbol(const std::string& data) {
        for (int i = 0; i < data.size(); ++i) {
                if (data[i] == '@') {
                        return true;
                }
        }
        std::cout << "Sorry, your email must contain @ symbol.\n";
        return false;
}

void Utility::SeparateEmail(const std::string& data, std::string& email_prefix, std::string& email_domain_name, std::string& email_top_level_domain) {
        int i{0};
        for ( ; (data[i] != '@') && (i < data.size()); ++i) {
                email_prefix.push_back(data[i]);
        }
        for (++i; (data[i] != '.') && (i < data.size()); ++i) {
                email_domain_name.push_back(data[i]);
        }
        for ( ; i < data.size(); ++i) {
                email_top_level_domain.push_back(data[i]);
        }
}

bool Utility::IsEmailPrefixValid(const std::string& prefix) {
        int prefix_size = prefix.size();
        if ((prefix_size < 6) || (prefix_size > 30)) {
                std::cout << "Sorry, your username must be between 6 and 30 characters long.\n";
                return false;
        }
        if ((prefix[0] < 'a') && (prefix[0] > 'z') && (prefix[0] < '0') && (prefix[0] > '9')) {
                std::cout << "Sorry, the first character of your username must be an ascii letter (a-z) or number (0-9).\n";
                return false;
        }
        if ((prefix[prefix_size - 1] < 'a') && (prefix[prefix_size - 1] > 'z') && (prefix[prefix_size - 1] < '0') && (prefix[prefix_size - 1] > '9')) {
                std::cout << "Sorry, the last character of your username must be an ascii letter (a-z) or number (0-9).\n";
                return false;
        }
        for (int i = 1; i < prefix.size() - 1; ++i) {
                if ((prefix[i] == '.') && (prefix[i+1] == '.')) {
                        std::cout << "Sorry, your username cannot contain consecutive periods(.)\n";
                        return false;
                }
                if (((prefix[i] < 'a') && (prefix[i] > 'z')) && ((prefix[i] < '0') && (prefix[i] > '9')) && (prefix[i] != '.')) {
                        std::cout << "Sorry, your username must only contain (a-z), (0-9) and dot(.)\n";
                        return false;
                }
        }
        return true;
}

bool Utility::IsDomainNameValid(const std::string& domain_name) {
        int domain_name_size = domain_name.size();
        if((domain_name.size() == 0) || (domain_name.size() > 63)) {
                std::cout << "Sorry, your domain name must be between 1 and 63 characters long.\n";
                return false;
        }
        if ((domain_name[0] == '-') || (domain_name[domain_name_size - 1] == '-')) {
                std::cout << "Sorry, your domain name cannot start or end with hyphen(-)";
                return false;
        }
        if ((domain_name[0] < 'a' && domain_name[0] > 'z')) {
                std::cout << "Sorry, your domain name must start with letters (a-z)";
        }
        for (int i = 0; i < domain_name_size; ++i) {
                if ((domain_name[i] != '-') && ((domain_name[i] < 'a') && (domain_name[i] > 'z')) &&
                   ((domain_name[i] < '0') && (domain_name[i] > '9'))) {
                        std::cout << "Domain name can only contain (a-z), (0-9) and hyphen(-)";
                        return false;
                }
        }
         return true;
}

bool Utility::IsTopLevelDomainValid(const std::string& top_level_domain) {
        if ((top_level_domain.size() < 2) || (top_level_domain.size() > 6)) {
                std::cout << "Sorry, your top-level domain must be between 2 and 6 characters long.\n";
                return false;
        }
        if (DoesTopLevelDomainAlreadyExist(top_level_domain)) {
                std::cout << "Your top-level domain already exists, therefore is valid.\n";
                return true;
        }
        std::cout << "Your top-level domain is valid.\n";
        return true;
}

bool Utility::DoesTopLevelDomainAlreadyExist(const std::string& domain) {
        std::ifstream inFile("top-level-domains.txt");
        if (!inFile.is_open()) {
                std::cout << "Sorry. Unable to open the file.";
        } else {
                std::string top_level_domain;
                while (std::getline(inFile, top_level_domain)) {
                        if (domain == top_level_domain) {
                                inFile.close();
                                return true;
                         }
               }
          }
        inFile.close();
        return false;
}

bool Utility::IsEmailUnique(const std::string& data) {
        std::ifstream inFile("existing-emails.txt");
        if (!inFile.is_open()) {
                std::cout << "Sorry. Unable to open the file.";
        } else {
                std::string email;
                while (std::getline(inFile,email)) {
                        if (data == email) {
                                inFile.close();
				std::cout << "Email already exists.\n";
                                return false;
                         }
               }
          }
        inFile.close();
        return true;
}

bool Utility::IsEmail(const std::string& data) {
        if (EmailContainsAtSymbol(data)) {
                std::string email_prefix;
                std::string email_domain;
                std::string email_top_level_domain;
                SeparateEmail(data, email_prefix, email_domain, email_top_level_domain);
        	if (IsEmailPrefixValid(email_prefix) && IsDomainNameValid(email_domain) &&
          	 IsTopLevelDomainValid(email_top_level_domain) && IsEmailUnique(data)) {
                }	return true;
        }
        return false;
}

bool Utility::IsURL(const std::string& data) {
	std::regex pattern("((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");
 	if (data.empty()) { 
		return false;
	}
	if(regex_match(data, pattern)) { 
		std::cout << "\nThe URL is valid.\n";
		return true;
	} else { 
		std::cout << "The URL is not valid.\n";
		return false;
	}
}



int main() {
        std::cout << "\nNumber validation\n";
        if (Utility::IsNumber("21")) { std::cout << "21 is a number\n"; }
        if (Utility::IsNumber("0")) { std::cout << "0 is a number\n"; }
        if (Utility::IsNumber("21.1")) { std::cout << "21.1 is a number\n"; }
        if (Utility::IsNumber("-21")) { std::cout <<  "-21 is a number\n"; }
        if (!Utility::IsNumber("21.1.1")) { std::cout << "21.1.1 is not a number\n"; }

	      std::cout << "\nBoolean value validation\n";
	      if (Utility::IsBoolean("true")) { std::cout << "true is a boolean value\n"; }
        if (Utility::IsBoolean("false")) { std::cout << "false is a boolean value\n"; }
        if (!Utility::IsBoolean("1")) { std::cout << "1 is not a boolean value\n"; }
        if (!Utility::IsBoolean("don't know")) { std::cout <<  "don't know is not a boolean value\n"; }

	      std::cout << "\nInteger Validation\n";
        if (Utility::IsInteger("20")) { std::cout << "20 is an integer\n"; }
        if (Utility::IsInteger("-20")) { std::cout << "-20 is not an integer\n"; }
        if (!Utility::IsInteger("1.1")) { std::cout << "1.1 is not an integer\n"; }
 	
        std::cout << "\nFloating Point Numbers Validation\n";
        if (Utility::IsFloat("1.1")) { std::cout << "1.1 is a float\n"; }
        if (Utility::IsFloat("-1.1")) { std::cout << "-1.1 is a float\n"; }
        if (!Utility::IsFloat("1")) { std::cout << "1 is not a float\n"; }
        if (!Utility::IsFloat(".1")) { std::cout << ".1 is not a float\n"; }

	     std::cout << "\nEmail Validation\n\n";
	     std::cout << "kristine.00@gmail.com\n";
	     if (Utility::IsEmail("kristine.00@gmail.com"));
	    std::cout << "\n123ame@example.jklsjalk\n";
	    if (Utility::IsEmail("123ame@example.jklsjalk"));
	    std::cout << "\nusernameatmail.ru\n";
	    if (Utility::IsEmail("isernameatmail.ru"));
	    std::cout << "\nus@gmail.com\n";
	    if (Utility::IsEmail("us@gmail.com"));

	  std::cout << "\nURL Validation\n";
	  std::cout << "\nhttps://www.code.com\n";
	  if (Utility::IsURL("https://www.codeschool.com\n"));

return 0;
}

