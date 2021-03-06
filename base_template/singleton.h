/*************************************************************************************************
* Copyright (C) 2006-2017 All rights reserved.
* @Version: 1.0
* @Created: 2017-01-19 08:52
* @Author: u203,cplusu203@gmail.com
* @Description: Singleton template without dependecies
*
* @History: Source code from boost_1_63_0/boost/container/detail/singleton.hpp
***************************************************************************************************/
#ifndef __BASE_TEMPLATE_SINGLETON_H__
#define __BASE_TEMPLATE_SINGLETON_H__

// The following helper classes are placeholders for a generic "singleton"
//  class.  The classes below support usage of singletons, including use in
//  program startup/shutdown code, AS LONG AS there is only one thread
//  running before main() begins, and only one thread running after main()
//  exits.
//
// This class is also limited in that it can only provide singleton usage for
//  classes with default constructors.
//

// The design of this class is somewhat twisted, but can be followed by the
//  calling inheritance. Let us assume that there is some user code that
//  calls "Singleton<T>::instance()". The following (convoluted)
//  sequence ensures that the same function will be called before main():
//  instance() contains a call to create_object.do_nothing()
// Thus, object_creator is implicitly instantiated, and create_object
//  must exist.
// Since create_object is a static member, its constructor must be
//  called before main().
// The constructor contains a call to instance(), thus ensuring that
//  instance() will be called before main().
// The first time instance() is called (i.e., before main()) is the
//  latest point in program execution where the object of type T
//  can be created.
// Thus, any call to instance() will auto-magically result in a call to
//  instance() before main(), unless already present.
// Furthermore, since the instance() function contains the object, instead
//  of the Singleton class containing a static instance of the
//  object, that object is guaranteed to be constructed (at the latest) in
//  the first call to instance().  This permits calls to instance() from
//  static code, even if that code is called before the file-scope objects
//  in this file have been initialized.

namespace base_template
{

// T must be: no-throw default constructible and no-throw destructible
template <typename T>
class Singleton
{
public:
    typedef T object_type;

    // If, at any point (in user code), Singleton<T>::instance()
    //  is called, then the following function is instantiated.
    static object_type & instance()
    {
        // This is the object that we return a reference to.
        // It is guaranteed to be created before main() begins because of
        //  the next line.
        static object_type obj;

        // The following line does nothing else than force the instantiation
        //  of Singleton<T>::create_object, whose constructor is
        //  called before main() begins.
        create_object.do_nothing();

        return obj;
    }

protected:

private:
    struct object_creator
    {
        // This constructor does nothing more than ensure that instance()
        //  is called before main() begins, thus creating the static
        //  T object before multithreading race issues can come up.
        object_creator()
        { Singleton<T>::instance(); }

        inline void do_nothing() const
        { }
    };
    static object_creator create_object;

    Singleton();
};

template <typename T>
typename Singleton<T>::object_creator Singleton<T>::create_object;

} // namespace base_template

#endif // end of __BASE_TEMPLATE_SINGLETON_H__


