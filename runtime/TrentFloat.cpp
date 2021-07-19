#include "TrentFloat.h"
#include "TrentInteger.h"
#include "TrentBoolean.h"
#include "TrentRuntime.h"

namespace trent
{
    TrentFloat::TrentFloat(float value)
        : d_value(value)
    {
    }
    
    const char* TrentFloat::ToString()
    {
        d_string_repr = std::to_string(this->d_value);
        return d_string_repr.c_str();
    }
    
    const char* TrentFloat::GetRuntimeName()
    {
        return "Float";
    }
    
    TrentObject* TrentFloat::Copy(bool delegate_ownership_to_runtime)
    {
        return MAKE_TRENT_FLOAT_SPEC_OWNERSHIP(delegate_ownership_to_runtime, d_value);
    }

    void TrentFloat::CopyFrom(TrentObject* other)
    {
        if (strcmp(other->GetRuntimeName(), "Float") != 0)
        {
            auto exception = TrentException("TrentFloat", "Cannot copy object buffer", "CopyObject::Error");
            exception.Raise();
            return;
        }

        this->d_value = reinterpret_cast<TrentFloat*>(other)->d_value;
    }
    
    TrentObject* TrentFloat::__operator_add(TrentObject* obj)
    {
        if (strcmp(obj->GetRuntimeName(), "Float") == 0)
            return MAKE_TRENT_FLOAT(this->d_value + reinterpret_cast<TrentFloat*>(obj)->d_value);

        if (strcmp(obj->GetRuntimeName(), "Int") == 0)
            return MAKE_TRENT_FLOAT(this->d_value + (float)reinterpret_cast<TrentInteger*>(obj)->GetValue());

        std::string ex_message = std::string("Cannot use + operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();
        auto exception = TrentException("TrentFloat", ex_message, "__operator_add::Error");
        exception.Raise();

        return TrentObject_Null;
    }
    
    TrentObject* TrentFloat::__operator_sub(TrentObject* obj)
    {
        if (strcmp(obj->GetRuntimeName(), "Float") == 0)
            return MAKE_TRENT_FLOAT(this->d_value - reinterpret_cast<TrentFloat*>(obj)->d_value);

        if (strcmp(obj->GetRuntimeName(), "Int") == 0)
            return MAKE_TRENT_FLOAT(this->d_value - (float)reinterpret_cast<TrentInteger*>(obj)->GetValue());

        std::string ex_message = std::string("Cannot use - operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();
        auto exception = TrentException("TrentFloat", ex_message, "__operator_sub::Error");
        exception.Raise();

        return TrentObject_Null;
    }
    
    TrentObject* TrentFloat::__operator_mul(TrentObject* obj)
    {
        if (strcmp(obj->GetRuntimeName(), "Float") == 0)
            return MAKE_TRENT_FLOAT(this->d_value * reinterpret_cast<TrentFloat*>(obj)->d_value);

        if (strcmp(obj->GetRuntimeName(), "Int") == 0)
            return MAKE_TRENT_FLOAT(this->d_value * (float)reinterpret_cast<TrentInteger*>(obj)->GetValue());

        std::string ex_message = std::string("Cannot use * operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();
        auto exception = TrentException("TrentFloat", ex_message, "__operator_mul::Error");
        exception.Raise();

        return TrentObject_Null;
    }
    
    TrentObject* TrentFloat::__operator_div(TrentObject* obj)
    {
        if (strcmp(obj->GetRuntimeName(), "Float") == 0)
            return MAKE_TRENT_FLOAT(this->d_value / reinterpret_cast<TrentFloat*>(obj)->d_value);

        if (strcmp(obj->GetRuntimeName(), "Int") == 0)
            return MAKE_TRENT_FLOAT(this->d_value / (float)reinterpret_cast<TrentInteger*>(obj)->GetValue());

        std::string ex_message = std::string("Cannot use / operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();
        auto exception = TrentException("TrentFloat", ex_message, "__operator_div::Error");
        exception.Raise();

        return TrentObject_Null;
    }
    
    TrentObject* TrentFloat::__operator_lt(TrentObject* obj)
    {
        if (strcmp(obj->GetRuntimeName(), "Float") == 0)
            return MAKE_TRENT_BOOLEAN((this->d_value < reinterpret_cast<TrentFloat*>(obj)->d_value));

        if (strcmp(obj->GetRuntimeName(), "Int") == 0)
            return MAKE_TRENT_BOOLEAN((this->d_value < (float)reinterpret_cast<TrentInteger*>(obj)->GetValue()));

        std::string ex_message = std::string("Cannot use < operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();
        auto exception = TrentException("TrentFloat", ex_message, "__operator_lt::Error");
        exception.Raise();

        return TrentObject_Null;
    }
    
    TrentObject* TrentFloat::__operator_gt(TrentObject* obj)
    {
        if (strcmp(obj->GetRuntimeName(), "Float") == 0)
            return MAKE_TRENT_BOOLEAN((this->d_value > reinterpret_cast<TrentFloat*>(obj)->d_value));

        if (strcmp(obj->GetRuntimeName(), "Int") == 0)
            return MAKE_TRENT_BOOLEAN((this->d_value > (float)reinterpret_cast<TrentInteger*>(obj)->GetValue()));

        std::string ex_message = std::string("Cannot use > operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();
        auto exception = TrentException("TrentFloat", ex_message, "__operator_gt::Error");
        exception.Raise();

        return TrentObject_Null;
    }
    
    TrentObject* TrentFloat::__operator_ltoe(TrentObject* obj)
    {
        if (strcmp(obj->GetRuntimeName(), "Float") == 0)
            return MAKE_TRENT_BOOLEAN((this->d_value <= reinterpret_cast<TrentFloat*>(obj)->d_value));

        if (strcmp(obj->GetRuntimeName(), "Int") == 0)
            return MAKE_TRENT_BOOLEAN((this->d_value <= (float)reinterpret_cast<TrentInteger*>(obj)->GetValue()));

        std::string ex_message = std::string("Cannot use <= operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();
        auto exception = TrentException("TrentFloat", ex_message, "__operator_ltoe::Error");
        exception.Raise();

        return TrentObject_Null;
    }
    
    TrentObject* TrentFloat::__operator_gtoe(TrentObject* obj)
    {
        if (strcmp(obj->GetRuntimeName(), "Float") == 0)
            return MAKE_TRENT_BOOLEAN((this->d_value >= reinterpret_cast<TrentFloat*>(obj)->d_value));

        if (strcmp(obj->GetRuntimeName(), "Int") == 0)
            return MAKE_TRENT_BOOLEAN((this->d_value >= (float)reinterpret_cast<TrentInteger*>(obj)->GetValue()));

        std::string ex_message = std::string("Cannot use >= operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();
        auto exception = TrentException("TrentFloat", ex_message, "__operator_gtoe::Error");
        exception.Raise();

        return TrentObject_Null;
    }
    
    TrentObject* TrentFloat::__operator_equequ(TrentObject* obj)
    {
        if (strcmp(obj->GetRuntimeName(), "Float") == 0)
            return MAKE_TRENT_BOOLEAN((this->d_value == reinterpret_cast<TrentFloat*>(obj)->d_value));

        if (strcmp(obj->GetRuntimeName(), "Int") == 0)
            return MAKE_TRENT_BOOLEAN((this->d_value == (float)reinterpret_cast<TrentInteger*>(obj)->GetValue()));

        std::string ex_message = std::string("Cannot use == operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();
        auto exception = TrentException("TrentFloat", ex_message, "__operator_equequ::Error");
        exception.Raise();

        return TrentObject_Null;
    }
    
    TrentObject* TrentFloat::__operator_notequ(TrentObject* obj)
    {
        if (strcmp(obj->GetRuntimeName(), "Float") == 0)
            return MAKE_TRENT_BOOLEAN((this->d_value != reinterpret_cast<TrentFloat*>(obj)->d_value));

        if (strcmp(obj->GetRuntimeName(), "Int") == 0)
            return MAKE_TRENT_BOOLEAN((this->d_value != (float)reinterpret_cast<TrentInteger*>(obj)->GetValue()));

        std::string ex_message = std::string("Cannot use != operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();
        auto exception = TrentException("TrentFloat", ex_message, "__operator_notequ::Error");
        exception.Raise();

        return TrentObject_Null;
    }
}
