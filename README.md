## **Game Development Framework - UTGS**

[![Watch My Video!](https://img.youtube.com/vi/ecjnQza6s0k/0.jpg)](https://www.youtube.com/watch?v=ecjnQza6s0k&list=PLAetEEjGZI7OUBYFoQvI0QcO9GKAvT1xT&index=15)


Game development framework developed in years 2001 - 2006, available as Open Source since 2005.

Nowadays, this project might be seen as inspiration to build something new.

Everyone is welcome to review the code, and learn the techniques that were used, and perhaps borrow some of the ideas into brand new projects in today's C++20.

Back in times, when this framework was developed, we only had Microsoft Visual Studio 6.

The support for C++ Templates was very limitted, and tricks proposed by Andrei Alexandrescu weren't working without additional retouches. Things like Signals, Functors, Type-Lists, and Variadic Templates were achieved with additional tricks and macros.

Nowadays, since C++11 and down to C++20 we have in the language: lambda functions, concepts, movable expiring values, and many other features, and the standard library now contains lots of great utility functions. It would have been completely different game development library if we could use those back then. 

But it's always great to appreciate the work that was done in the past, to understand where all the new features came from. This game development library would probably be one of many projects out there where tricks for C++ Templates were used, and that was driving the need to make improvements in the language. We have seen new features being introduced as a Boost library, and we were looking forward to the future where they become part of standard library, and it took more than 10 years to happen.


#### ***List of People***

Manager:
- Jacek Trzmiel - Head of Development

Author: 
- Sonia Kolasinska (https://github.com/sadhbh-c0d3) - Directly Responsible

Additional contributions:
- Jacek Trzmiel
- Janusz Gregorczyk
- Sebastian Szymanski

Special thanks for inspiration to:
- Jacek Trzmiel
- Andreas Huber
- Andrei Alexandrescu

I want to add that without the inspiration from Andrei and Andreas this project wouldn't be possible.

I also want to thank my manager Jacek for always inspiring me to solve difficult problems.

#### ***List of Features***

- Game Development Framework
- Powerful yet easy to use GUI Toolkit optimized for Video Games
- Supported Ogg / Vorbis, DirectShow, OpenGL
- Written in C++98
- Modular Architecture
- Platform Abstraction
- Virtual File System
- File Encryption
- Function Objects, Signals (Multicast Delegate)
- Resource Definition Language (XML)
- Scripting Functional Programming Language

#### ***Documentation***

Serve the `www` folder over HTTP, e.g. using Python `http.server`:
```
python -m http.server --directory www
```

And then in your web browser open <http://localhost:8000/>

And you should see documentation.

### **Architecture**

The framework consists of several layers of abstraction and numerous components.

#### ***Multiple Layers of Abstraction***
There is platform layer where system API is abstracted using Application and Window classes: <https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/System>

There is multiple graphics abstraction layers:

- rendering device layer (DirectX, OpenGL, there also was RenderWare Graphics in non OpenSource version): <https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/Graphic/Device>
- file O/I layer (loading and writing images BMP, TGA, PNG): <https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/Graphic/FileIO>
- high level image planes (device independent abstraction: image, buffer, animation, etc): <https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/Graphic/Planes>
- pixel transfer algorithms (used depending on what was target buffer capabilities, e.g. OpenGL would use glPixelStore (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/Graphic/Device/GL/GLTexture.cpp#L61>), which means we send pixels from our buffer in system memory into OpenGL, while DirectX Surface would provide Lock method, and we’d write pixels directly to that surface without allocating buffer in system memory. In that second case we would be doing pixel format conversions too using C++ templates): <https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/Graphic/Transfer>
- pixel drawing algorithms (e.g. draw anti-aliased line, or polygon using scan-line rendering algorithm): <https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/Graphic/Pixel>
- there’s also linear algebra utilities as C++ templates: <https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/Graphic/Linear.h>

There is video playback capability via DirectShow (<https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/Movie/DShow>) also taking into account two possibilities, where we want to tell DirectShow to render directly onto DirectX surface, or when we want to render into buffer in system memory and send to OpenGL.

The music playback in Ogg/Vorbis format is also there (<https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/Movie/Ogg>)

The sound system also provided using multiple abstraction layers:

- sound device layer: <https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/Sound/Device>
- file I/O layers: <https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/Sound/FileIO>
- high level abstraction layer: <https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/Sound>
#### ***Virtual File System & Encrypted File System***
The framework provided virtual file system too, and it allows you to mount number of (sub) file systems at number of mount locations. The mounted locations, or sub-folders may overlap between (sub) file systems. That is a feature, which allows us to provide patches, i.e. when game is distributed with its assets, and patch is required a new file containing encrypted (sub) file system is shipped, and that (sub) file system then contains files that over shadow the ones in originally shipped product. See <https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/File>
#### ***Super Powerful GUI Toolkit***
The core strength of this framework is powerful GUI toolkit: <https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/GUI>

I took inspiration from GTK when designing this GUI toolkit.

The GUI toolkit consists of several layers too.

- There is a layer that represents the device context <https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/GUI/Display>
- There is an abstraction of the GUI workspace <https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/GUI/Workspace.h>, which organises GUI into 4 layers: ballon hints, menus, popup windows (dialogs), and desktop. This helps manage correct GUI behavior, i.e. ballon hints shall be always on top, then menus, then popups (dialogs), and then desktop (main GUI).
- The binding between device context and workspace is achieved via GUIMaster class <https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/GUI/GUIMaster.h>
- The GUI itself is built of widgets: <https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/GUI/Widgets>

The GUI widgets are very interesting beings in this framework.

There is Widget class, that serves as base for all widget classes (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/GUI/Widgets/Widget.h>), and then widgets that are reactive have another common base class ActiveWidget (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/GUI/Widgets/ActiveWidget.h>), and if widget allows multiple choices there is EnumWidget (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/GUI/Widgets/EnumWidget.h>).

Widgets are composed into hierarchy, and that is achieved using CompoundWidget as base class (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/GUI/Widgets/CompoundWidget.h>), and there is HubWidgets (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/GUI/Widgets/HubWidget.h>) that forwards signals to children, and FrameWidgets (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/GUI/Widgets/FrameWidget.h>) that extends HubWidgets with visual frame surrounding the widgets within.

These are base classes, and they are then extended by numerous widgets.

The reactive widgets have separated logic aspect from visual aspect.

And for instance there is IconWidget (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/GUI/Widgets/IconWidget.h>) that draws an image (actually it renders one of the skins from a set), and there is CaptionWidget (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/GUI/Widgets/CaptionWidget.h>) that draws an image with text (overlaid or aligned to the right or left of the image).

Logic for those widgets lives in separate classes, and LogicWidget is the base class for the logic (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/GUI/Logic/LogicWidget.h>). It inherits Widgets as virtual base class, because we create mix-in diamond shape hierarchy. For example we have ButtonLogic (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/GUI/Logic/ButtonLogic.h>) and we mix that with IconWidget (mentioned above), and we get a widget that draws an image, and that image is selected based on the logic of the button, i.e. if we hover mouse over we will obtain highlighted image, and when we press mouse button we will get pressed button image. The widget composed of IconWidget and ButtonLogic is IconButton (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/GUI/Widgets/IconButton.h>), which uses TemplateWidget C++ template to produce new class from two mix-in classes.

Another great feature of GUI Toolkit are Relations (<https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/GUI/Relations>). A Relation is a transient algorithm connecting two or more widgets, that lives only until destruction of Relation. This allows for temporary connections between widgets. The relations can for instance connect scrollbar to scrollable content. The connections are realised via Signals.
#### ***Signals & Functors***
The whole GUI toolkit wouldn’t be possible without this fantastic creatures called signals. We need to remember this framework was developed for C++98 at times of Microsoft Visual Studio 6, when we didn’t have std::function nor lambda in C++ (<https://github.com/sadhbh-c0d3/UTGS/tree/main/Useless/Functional>).

I have been inspired by article I found is C++ Journal about Functors. I have created functor classes, which wrapped a pointer to an object, and to member method, and also contained packed arguments (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/Functional/PackedFunctor.h>).

The way signals work is they have their arguments stored within Signal instance, and when raising signal those arguments are updated, and if attached packed functors are packed with references to those arguments, then those functors will receive value from signal. It was up to user to pack functors in whichever way user wanted. User could pack functor with expression that packed arguments and some constants with operator (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/Functional/EquationHolder.h>). A condition could be also packed into functor (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/Functional/ConditionalFunctor.h>).
#### ***Asynchronous Calls***
There is also capability for asynchronous calls implemented by DelayedExecutor (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/Functional/DelayedExecutor.h>).

Background music playback required to decode next buffer every so often, and since games were single-threaded this was scheduled as asynchronous task. If it happened that GUI thread stalled, then music stopped playing until GUI thread resumed.

The destruction of widgets was always garbage collected, because it was happening on signal events, and destroying widget while signal is being handled would cause crash. Widget was added to garbage collection, and then asynchronous task would destroy garbage when GUI was idle.
#### ***Building GUI from XML Resources***
An XML parser (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/XML/XMLParser.h>) allowed to parse XML files containing then definitions of GUI components. There is a factory of GUI components (<https://github.com/sadhbh-c0d3/UTGS/blob/main/Useless/XML/Resources/Resources.h>) which converts XML elements into actual live GUI.

If you look at my C++ Templates I was using **Tag Dispatch** method (that I think I borrowed from Boost source code).
#### ***Functional Programming Language***
Building GUI from XML files only increased appetite for more advanced scripting than just building static GUI. Initially we wanted some minimal interaction so that graphic designer could switch between screens. Later on more and more real programming language features were added, including variable declarations, constants, map and fold.

The language is using ExecutionState and IChunk (<https://github.com/sadhbh-c0d3/UTGS/blob/main/koolib/xml/XMLProgram.h>). The IChunk represents chunks (any value, function, object, module) in the language, while ExecutionState has dictionary of chunks, which acts as local variables, and it has reference to parent ExecutionState. During execution if fetching variable is requested, the language will first search current ExecutionState for that variable, and if not found will go and look at parent and then ancestors until it reaches global scope. This is significantly different way of variable lookup than in more traditional programming languages.

Additionally language provides compiler (<https://github.com/sadhbh-c0d3/UTGS/blob/main/koolib/xml/XMLKompiler.h>), which instead of executing XML nodes on the go, it builds an abstract syntax tree (AST). The resulting AST is executable and is stored as IChunk (as everything in this language), and assigned to a variable in the current ExecutionState. Later that variable can be executed, and that will execute whole AST. The compiler does several optimisations to speed up execution of the AST. For instance it optimises constants, and removes unnecessary stack frame creation. Also iteration speed is improved by optimising map and fold.

The language in general has no imperative constructs except set element, which changes the value of the mutable variable. All loops must be made via map or fold.

I used this language successfully to write the full logic of vide game for kids (<https://www.gamepressure.com/games/postman-pat/z81986>).
#### ***Dynamic Module Loader***
Although language was fast and on 800MHz machine could execute 100k AST nodes per second, some more performance critical code would need to be written in C++, and for that a dynamic module loader was added (<https://github.com/sadhbh-c0d3/UTGS/tree/main/Dynamo>). That feature was only experimental, and never used in any game.

