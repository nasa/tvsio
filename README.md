TVS-IO is a Core Flight Software (cFS) app that enables two-way communication between cFS's Software Bus Network (SBN) and Trick simulations.

Visit the [TVS-IO App Wiki](https://github.com/nasa/tvsio/wiki) for information on how TVS-IO works, cloning, configuring, building, and running.

For a sample of how TVS-IO can be incorporated into a cFS project, see [tvsio-demo](https://github.com/nasa/tvsio-demo)

This app can be configured as any other CFS app would in a mission.  However, the generated code often has to include several other apps message type header files. To facilitate this configuration, the cmake in the app will look for a cmake configuration file at <target_defs>/tvsio_config.cmake. There the user can specify the list of \*.tvm files as well as any include or definition directives to pass to the build configuration for compiling the generated code. A best practice might be to put the \*.tvm files in a subdirectory under <target_defs> but the cmake allows projects to place it anywhere. See sample_tvsio_config.cmake for an example <target_defs>/tvsio_config.cmake implementation (Taken from tvsio-demo).

TVS-IO is released under the NASA Open Source Agreement Version 1.3 [license](LICENSE).
