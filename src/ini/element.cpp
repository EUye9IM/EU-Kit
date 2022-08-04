#include <ini/ini.h>

using std::string;
namespace EUkit::ini {

// Field
Field::Field() {}
Field::Field(const std::string &section, const std::string &keyname) {
	this->section = section;
	this->keyname = keyname;
}
bool Field::operator<(const Field &right) const {
	if (this->section < right.section)
		return true;
	if (this->section > right.section)
		return false;
	if (this->keyname < right.keyname)
		return true;
	return false;
}

// Element
Element::Element() {
	type = Type::INT;
	section = "";
	keyname = "";
	default_value.Int = 0;
	check_func.Int = nullptr;
	is_exist = false;
	real_value.Int = 0;
};
Element::Element(Type type, const std::string &section,
				 const std::string &keyname, const Value &default_value,
				 CheckFunc check_func, bool is_exist,
				 const Value &real_valuel) {
	this->type = type;
	this->section = section;
	this->keyname = keyname;
	this->default_value = default_value;
	this->check_func = check_func;
	this->is_exist = is_exist;
	this->real_value = real_value;
}
void Element::set(Type type, const std::string &section,
				  const std::string &keyname, const Value &default_value,
				  CheckFunc check_func, bool is_exist,
				  const Value &real_valuel) {
	this->type = type;
	this->section = section;
	this->keyname = keyname;
	this->default_value = default_value;
	this->check_func = check_func;
	this->is_exist = is_exist;
	this->real_value = real_value;
}
} // namespace EUkit::ini