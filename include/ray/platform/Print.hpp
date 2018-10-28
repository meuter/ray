#pragma once

#include <iostream>
#include <sstream>
#include <boost/format.hpp>

namespace ray { namespace platform {
    
    namespace details 
    {
        inline void printHelper(std::ostream &out) { (void)out; }
        
        template <typename T, typename... Args>
        inline void printHelper(std::ostream &out, T &&value, Args&&... args) { printHelper(out << std::forward<T>(value), std::forward<Args>(args)...); }        

        inline std::string fprintHelper(boost::format &formatter) { return boost::str(formatter); }
        
        template <class T, class... Args>
        inline std::string fprintHelper(boost::format &formatter, T &&value, Args &&... args) { return fprintHelper(formatter % std::forward<T>(value), std::forward<Args>(args)...); }    
    }

    template <typename... Args>
    inline void print(std::ostream &out, Args&&... args) { details::printHelper(out, std::forward<Args>(args)...); }
    
    template <typename... Args>
    inline void print(std::stringstream &out, Args&&... args) { details::printHelper(out, std::forward<Args>(args)...); }
    
    template <typename... Args>
    inline void print(Args&&... args) { details::printHelper(std::cout, std::forward<Args>(args)...); }
    
    template <typename... Args>
    inline void println(std::ostream &out, Args&&... args) { details::printHelper(out, std::forward<Args>(args)..., '\n'); }

    template <typename... Args>
    inline void println(std::stringstream &out, Args&&... args) { details::printHelper(out, std::forward<Args>(args)..., '\n'); }

    template <typename... Args>
    inline void println(Args&&... args) { details::printHelper(std::cout, std::forward<Args>(args)..., '\n'); }
    
    template <typename... Args>
    inline std::string cat(Args&&... args) { std::stringstream buffer; details::printHelper(buffer, std::forward<Args>(args)...); return buffer.str(); }
    
    template <typename... Args>
    void fprint(std::ostream &out, const std::string &format, Args &&... args)
    {
        boost::format formatter(format);
        out << details::fprintHelper(formatter, std::forward<Args>(args)...);
    }
    
    template <typename... Args>
    void fprint(std::string const &format, Args &&... args) { fprint(std::cout, format, std::forward<Args>(args)...); }
    
    template <typename... Args>
    void fprintln(std::ostream &out, std::string const &format, Args &&... args) { fprint(out, format + "\n", std::forward<Args>(args)...); }
    
    template <typename... Args>
    void fprintln(std::string const &format, Args &&... args) { fprintln(std::cout, format, std::forward<Args>(args)...);  }
    
    template <typename... Args>
    std::string fmt(std::string const &format, Args &&... args)
    {
        boost::format formatter(format);
        return details::fprintHelper(formatter, std::forward<Args>(args)...);
    } 

}}

