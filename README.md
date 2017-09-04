# particle_system

This project is a system of particles, implemented in C++ with OpenGL and GLFW for the display, and OpenCL for the particle handling,
made to run on a mac os system.

In my opinion, this school project is one of the more interesting I worked on, as it touches the problem of parallel computing.
Indeed, this program will display as many particles as you give it in parameters, and will handle a 60fps rendering
of 1 million moving particles as colored points.

Under regular programming methods, this would not be possible, as at each frame, we have to update the position of every
particle according to a gravity point, and apply a speed to it to have some manner of animation. If it were to be
coded as we would any C or C++ program, processed line after line, it would be difficult to go beyond the 10 fps (in 2017 that is).

That is why for this project, we have to use OpenCL. At first, I thought that OpenCL would be some faster OpenGL-like library to animate
my particles, but I soon realized that I was utterly wrong.

OpenCL is a library that allows to easily parallelize tasks on datas. That means it allows to do almost anything in non-linear processing.

If you have a computer with no graphic card and 10 processors, or a computer with one processor and 10 graphic cards, it does not matter
to OpenCL, as the library will take every "processing core" and treat them as "devices". It will then handle a task queue for the
tasks you give it, and process them in parallel.

That is how we can animate 1 million particles in an instant, as almost all of today's graphic cards architectures are made to handle
tasks in parallel.

For this, we have to prepare our code on our "host", and send our datas and tasks to our "devices", and then wait for them to be
processed.
However, the subject is very extensive, and I strongly recommend [*The OpenCL Programming Book*](https://www.fixstars.com/en/opencl/book/)
to learn more about parallel computing and OpenCL.

Video of the program running:

[![Particle System video link](https://img.youtube.com/vi/NLhrn4jX7YM/0.jpg)](https://www.youtube.com/watch?v=NLhrn4jX7YM)
