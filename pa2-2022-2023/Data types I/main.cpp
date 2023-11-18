#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

typedef unsigned int UI;

// ================================================================= //


class CPP_TextFile {
public:
    virtual inline ~CPP_TextFile() = default;
    virtual inline bool operator == (const CPP_TextFile &) = 0;
    virtual inline bool operator != (const CPP_TextFile &) = 0;
    virtual inline const size_t getSize() const = 0;
    virtual inline CPP_TextFile * duplicate() const = 0;
    virtual inline void render (std::ostream &) const = 0;
    
};

// ================================================================= //

class CDataTypeInt : public CPP_TextFile {
public:
    
    inline void render (std::ostream & oss) const override {oss << "int";}
    inline const size_t getSize() const override {
        static const unsigned short int penis_size = 4;
        return penis_size;
    }
    inline bool operator == (const CPP_TextFile & int_var) override {
        if (typeid(*this) == typeid(int_var)) return true;
        else return false;
    }
    inline bool operator != (const CPP_TextFile & int_var) override {
        if (typeid(this) != typeid(int_var)) return true;
        else return false;
    }
    inline CDataTypeInt * duplicate() const override {return new CDataTypeInt(*this);}
    
};

// ================================================================= //

class CDataTypeDouble : public CPP_TextFile {
public:
    
    inline void render (std::ostream & oss) const override {oss << "double";}
    inline const size_t getSize() const override {return 8;}
    inline bool operator == (const CPP_TextFile & dob_var) override {
        if (typeid(*this) == typeid(dob_var)) return true;
        else return false;
    }
    inline bool operator != (const CPP_TextFile & dob_var) override {
        if (typeid(*this) != typeid(dob_var)) return true;
        else return false;
    }
    inline CDataTypeDouble * duplicate() const override {return new CDataTypeDouble(*this);}
    
};

// ================================================================= //

class CDataTypeEnum : public CPP_TextFile {
public:
    std::deque<std::string> list_elem;
    
    inline void render (std::ostream & oss) const override {
        oss << "enum\n{\n";
        for (size_t a=0; a<this->list_elem.size(); a++) {
            if (a+1 != this->list_elem.size()) {
                oss << this->list_elem[a] << ",\n";
            } else {
                oss << this->list_elem[a] << "\n";
            }
        }
        oss << "}";
    }
    
    inline const size_t getSize() const override {return 4;}
    inline CDataTypeEnum * duplicate() const override {return new CDataTypeEnum(*this);}
    
    inline CDataTypeEnum & add(const std::string & name) {
        for (auto ptr=this->list_elem.begin(); ptr!=this->list_elem.end(); ptr++) {
            if (*ptr == name)
                throw std::invalid_argument("Duplicate enum value: " + name);
        }
        this->list_elem.emplace_back(name);
        return * this;
    }
    
    inline bool operator == (const CPP_TextFile & enum_var) override  {
        const CDataTypeEnum * value = dynamic_cast<const CDataTypeEnum *>(&enum_var);
        if (value == nullptr || value->list_elem != this->list_elem) return false;
        else {
            auto val_ptr = value->list_elem.begin();
            auto enum_ptr = this->list_elem.begin();
            while (val_ptr != value->list_elem.end()) {
                if (*val_ptr != *enum_ptr)
                    return false;
                
                enum_ptr++;
                val_ptr++;
            }
        }
        return true;
    }
    
    inline bool operator != (const CPP_TextFile & enum_var) override {
        const CDataTypeEnum * value = dynamic_cast<const CDataTypeEnum *>(&enum_var);
        if (value == nullptr || value->list_elem != this->list_elem) return true;
        else {
            auto val_ptr = value->list_elem.begin();
            auto enum_ptr = this->list_elem.begin();
            while (val_ptr != value->list_elem.end()) {
                if (*val_ptr == *enum_ptr) {
                    return false;
                }
                val_ptr++;
                enum_ptr++;
            }
        }
        return true;
    }
};

// ================================================================= //

class CDataTypeStruct : public CPP_TextFile {
public:
    std::deque<std::tuple<std::string, CPP_TextFile *>> list;
    
    CDataTypeStruct() = default;
    
    ~CDataTypeStruct() {
        for (auto & elem : list) {
            delete std::get<1>(elem);
        }
//        for (auto ptr=this->list.begin(); ptr!=this->list.end(); ptr++) {
//            auto smth = ptr;
//            ptr++;
//            delete &smth;
//        }
    }
    
    inline CDataTypeStruct (const CDataTypeStruct & data) {
        for (auto ptr=data.list.begin(); ptr!=data.list.end(); ptr++) {
            this->list.emplace_back(std::get<0>(*ptr), std::get<1>(*ptr)->duplicate());
        }
    }
    
    inline void render (std::ostream & oss) const override {
        oss << "struct\n{\n";
        for (auto ptr=this->list.begin(); ptr!=this->list.end(); ptr++) {
            std::get<1>(*ptr)->render(oss);
            oss << std::get<0>(*ptr);
            oss << ";\n";
        }
        oss << "}";
    }
    
    inline CDataTypeStruct * duplicate() const override {
        return new CDataTypeStruct(*this);
    }
    
    // std::deque<std::tuple<std::string, CLASS *>> list
    inline const size_t getSize() const override {
        size_t size = 0;
        for (auto ptr=this->list.begin(); ptr!=this->list.end(); ptr++) {
            size += std::get<1>(*ptr)->getSize();
        }
        return size;
    }
    
    inline CDataTypeStruct & addField(const std::string & field_, const CPP_TextFile & cp) {
        auto ptr=this->list.begin();
        for (; ptr!=this->list.end(); ptr++)
            if (std::get<0>(*ptr) == field_)
                throw std::invalid_argument("Duplicate field: " + field_);
        
        CPP_TextFile * tmp = cp.duplicate();
        this->list.emplace_back(field_, tmp);
        return * this;
    }
    
    inline CPP_TextFile & field(const std::string & name) const {
        auto ptr=this->list.begin();
        for (; ptr!=this->list.end(); ptr++) {
            if (std::get<0>(*ptr) == name) break;
            else if (ptr == this->list.end()-1 && std::get<0>(*ptr) != name)
                throw std::invalid_argument("Unknown field: " + name);
        }
        return *std::get<1>(*ptr);
    }
    
    inline bool operator == (const CPP_TextFile & struct_var) override {
        const CDataTypeStruct * value = dynamic_cast<const CDataTypeStruct *>(&struct_var);
        if (value == nullptr || value->list.size() != this->list.size()) return false;
        else {
            auto value_ptr = value->list.begin();
            auto struct_ptr = this->list.begin();
            while (value_ptr != value->list.end()) {
                if (typeid(*std::get<1>(*value_ptr)) != typeid(*std::get<1>(*struct_ptr)))
                    return false;
                value_ptr++;
                struct_ptr++;
            }
        }
        return true;
    }
    
    inline bool operator != (const CPP_TextFile & struct_var) override {
        const CDataTypeStruct * value = dynamic_cast<const CDataTypeStruct *>(&struct_var);
        if (value == nullptr || value->list.size() != this->list.size()) return true;
        else {
            auto value_ptr = value->list.begin();
            auto struct_ptr = this->list.begin();
            while (value_ptr != value->list.end()) {
                if (typeid(*std::get<1>(*value_ptr)) != typeid(*std::get<1>(*struct_ptr))) {
                    return true;
                }
                value_ptr++;
                struct_ptr++;
            }
        }
        return true;
    }
};

inline std::ostream & operator << (std::ostream & oss, const CPP_TextFile & cl) {
    cl.render(oss);
    return oss;
}

// ================================================================= //

#ifndef __PROGTEST__

static bool whitespaceMatch(const string &a, const string &b) {

    return true;
}

template<typename T_> static bool whitespaceMatch(const T_ &x, const string &ref) {
    ostringstream oss;
    oss << x;
    return whitespaceMatch(oss.str(), ref);
}

int main(void) {

    CDataTypeStruct a = CDataTypeStruct().
            addField("m_Length", CDataTypeInt()).
            addField("m_Status", CDataTypeEnum().
            add("NEW").
            add("FIXED").
            add("BROKEN").
            add("DEAD")).
            addField("m_Ratio", CDataTypeDouble());

    CDataTypeStruct b = CDataTypeStruct().
            addField("m_Length", CDataTypeInt()).
            addField("m_Status", CDataTypeEnum().
            add("NEW").
            add("FIXED").
            add("BROKEN").
            add("READY")).
            addField("m_Ratio", CDataTypeDouble());

    CDataTypeStruct c = CDataTypeStruct(). 
            addField("m_First", CDataTypeInt()).
            addField("m_Second", CDataTypeEnum().
            add("NEW").
            add("FIXED").
            add("BROKEN").
            add("DEAD")).
            addField("m_Third", CDataTypeDouble());

    CDataTypeStruct d = CDataTypeStruct().
            addField("m_Length", CDataTypeInt()).
            addField("m_Status", CDataTypeEnum().
            add("NEW").
            add("FIXED").
            add("BROKEN").
            add("DEAD")).
            addField("m_Ratio", CDataTypeInt());
    assert (whitespaceMatch(a, "struct\n"
                               "{\n"
                               "  int m_Length;\n"
                               "  enum\n"
                               "  {\n"
                               "    NEW,\n"
                               "    FIXED,\n"
                               "    BROKEN,\n"
                               "    DEAD\n"
                               "  } m_Status;\n"
                               "  double m_Ratio;\n"
                               "}"));

    assert (whitespaceMatch(b, "struct\n"
                               "{\n"
                               "  int m_Length;\n"
                               "  enum\n"
                               "  {\n"
                               "    NEW,\n"
                               "    FIXED,\n"
                               "    BROKEN,\n"
                               "    READY\n"
                               "  } m_Status;\n"
                               "  double m_Ratio;\n"
                               "}"));

    assert (whitespaceMatch(c, "struct\n"
                               "{\n"
                               "  int m_First;\n"
                               "  enum\n"
                               "  {\n"
                               "    NEW,\n"
                               "    FIXED,\n"
                               "    BROKEN,\n"
                               "    DEAD\n"
                               "  } m_Second;\n"
                               "  double m_Third;\n"
                               "}"));

    assert (whitespaceMatch(d, "struct\n"
                               "{\n"
                               "  int m_Length;\n"
                               "  enum\n"
                               "  {\n"
                               "    NEW,\n"
                               "    FIXED,\n"
                               "    BROKEN,\n"
                               "    DEAD\n"
                               "  } m_Status;\n"
                               "  int m_Ratio;\n"
                               "}"));

    assert (a != b);
    assert (a == c);
    assert (a != d);
    assert (a.field("m_Status") == CDataTypeEnum().add("NEW").add("FIXED").add("BROKEN").add("DEAD"));
    assert (a.field("m_Status") != CDataTypeEnum().add("NEW").add("BROKEN").add("FIXED").add("DEAD"));
    assert (a != CDataTypeInt());
    assert (whitespaceMatch(a.field("m_Status"), "enum\n"
                                                 "{\n"
                                                 "  NEW,\n"
                                                 "  FIXED,\n"
                                                 "  BROKEN,\n"
                                                 "  DEAD\n"
                                                 "}"));

    CDataTypeStruct aOld = a;
    b.addField("m_Other", CDataTypeDouble());

    a.addField("m_Sum", CDataTypeInt());

    assert (a != aOld);
    assert (a != c);
    assert (aOld == c);
    assert (whitespaceMatch(a, "struct\n"
                               "{\n"
                               "  int m_Length;\n"
                               "  enum\n"
                               "  {\n"
                               "    NEW,\n"
                               "    FIXED,\n"
                               "    BROKEN,\n"
                               "    DEAD\n"
                               "  } m_Status;\n"
                               "  double m_Ratio;\n"
                               "  int m_Sum;\n"
                               "}"));

    assert (whitespaceMatch(b, "struct\n"
                               "{\n"
                               "  int m_Length;\n"
                               "  enum\n"
                               "  {\n"
                               "    NEW,\n"
                               "    FIXED,\n"
                               "    BROKEN,\n"
                               "    READY\n"
                               "  } m_Status;\n"
                               "  double m_Ratio;\n"
                               "  double m_Other;\n"
                               "}"));

    c.addField("m_Another", a.field("m_Status"));

    assert (whitespaceMatch(c, "struct\n"
                               "{\n"
                               "  int m_First;\n"
                               "  enum\n"
                               "  {\n"
                               "    NEW,\n"
                               "    FIXED,\n"
                               "    BROKEN,\n"
                               "    DEAD\n"
                               "  } m_Second;\n"
                               "  double m_Third;\n"
                               "  enum\n"
                               "  {\n"
                               "    NEW,\n"
                               "    FIXED,\n"
                               "    BROKEN,\n"
                               "    DEAD\n"
                               "  } m_Another;\n"
                               "}"));

    d.addField("m_Another", a.field("m_Ratio"));

    assert (whitespaceMatch(d, "struct\n"
                               "{\n"
                               "  int m_Length;\n"
                               "  enum\n"
                               "  {\n"
                               "    NEW,\n"
                               "    FIXED,\n"
                               "    BROKEN,\n"
                               "    DEAD\n"
                               "  } m_Status;\n"
                               "  int m_Ratio;\n"
                               "  double m_Another;\n"
                               "}"));

    assert (a.getSize() == 20);
    assert (b.getSize() == 24);
    try {
        a.addField("m_Status", CDataTypeInt());
        assert ("addField: missing exception!" == nullptr);
    }
    catch (const invalid_argument &e) {
        assert (e.what() == "Duplicate field: m_Status"sv);
    }

    try {
        cout << a.field("m_Fail") << endl;
        assert ("field: missing exception!" == nullptr);
    }
    catch (const invalid_argument &e) {
        assert (e.what() == "Unknown field: m_Fail"sv);
    }

    try {
        CDataTypeEnum en;
        en.add("FIRST").
                add("SECOND").
                add("FIRST");
        assert ("add: missing exception!" == nullptr);
    }
    catch (const invalid_argument &e) {
        assert (e.what() == "Duplicate enum value: FIRST"sv);
    }

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
