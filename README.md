**IMPORTANT NOTICE** This framework is currently under heavy development. For
now, the use of this framework is not recommended for real projects.

-------

Icarus Framework
================

Aiming for productivity and performance, the Icarus Framework is a Web
Development Framework written in C/C++ with some ideas compatible with the
[Play Framework](https://www.playframework.com/) (Scala and Java).

Based on Model-View-Controller, the framework provides a facilities to integrate
with the [SOCI](http://soci.sourceforge.net/) (database access library that
provides backend for many servers as MySQL, Oracle, PostgreSQL).

For a better setup with different technologies and plataforms, the FastCGI
is used for integration with HTTP Servers. Hence, the framework does not need
to work with the complexity of a full web server implementation. Although it
still can be applied in the future, as another method.

All the development of a project using this framework (not mandatory) will used
CMake as the default build system. If you use another build tool, it can be used
too, but is not default configured. You are very welcome to contribute.

Performance
-----------

Right now, we have no idea about the performance of the current implementation.
Once we get a stable version done, tests will be performed in order to analyze
and improve the general speed.

Basic flow
----------

```
        Internet                               Icarus
     ~~~~~~~~~~~~~                     ~~~~~~~~~~~~~~~~~~~~
        Request          FastCGI
+--------+    +------------+    +-------------+    +------------+
| Client | => | HttpServer | => | Application | => | Dispatcher |
+--------+    +------------+    +-------------+    +------------+
                                                         ||
                                                         \/
                       +------+    +------------+    +--------+
                       | View | <= | Controller | <= | Routes |
                       +------+    +------------+    +--------+
```

Routes
------

The `routes` is a simple text file that binds URIs with controller methods.
Internally, at the pre-build stage of the project compilation, the framework
will parse and transform the `routes` into C++ code. Afterwards, the temporary
C++ code will be compiled into the main executable (export to a library).

### Basic

The basic structure is composed by the HTTP method, the corresponding URI (with
a params description support) and the controller method that will handle that
request.

The rules are checked in sequence from the top to bottom.

```
GET         /                           controllers::index::def()
GET         /about                      controllers::static_pages::about()
GET         /<user:[a-z_]+>/dashboard   controllers::user::dashboard(string username)
GET         /user/create                controllers::user::form()
POST        /user/create                controllers::user::create()
```

### Groups

Another feature implemented are groups. Where

```
on /<user>
{
	GET			/                       controllers::user::dashboard(std::string username)
	GET			/posts                  controllers::user::posts(std::string username)
	GET			/posts/<month:[0-9]+>   controllers::user::posts(std::string username, unsigned int month)
}
```

Models
------

The model part still under development.

Views
-----

The view system is HTML/C++ (`.hpp.html`) based, which can receive parameters as input.

```html
@(std::string title, unsigned int count)
<html>
<head>
    <title>@title</title>
</head>
<body>
    Sequence: @{
    for (unsigned int i = 0; i < count; i++)
    {
        out << " " << count;
    }
}
</body>
</html>
```

The view is basically parsed and converted to a C++ class as an static method.
It makes the template system very efficient, once it is a compiled code.

Controllers
-----------

The controllers have a , general, simple implementation.

```c++
namespace controllers
{
class index
{
public:
	static icarus::result def()
	{
		icarus::result r("<html>A string can be used as a content.</html>");
		return r;
	}
};
}
```

Below, a example using a view:

```c++
namespace controllers
{
class signin
{
public:
	static icarus::result sign_in_form()
	{
		icarus::result r;
		views::login::signin(r);
		return r;
	}
};
}
```

Other facilities
----------------
Other facilities as HTTP Session and WebSockets will be implemented in future
versions.
