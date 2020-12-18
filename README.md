This is the TVS-IO (CFS embedded) app repo.


Visit the [TVS-IO App Wiki](https://esgl-gitlab.jsc.nasa.gov/tricksbn/tvsio_app/-/wikis/home) for information on how TVS-IO works, cloning, configuring, building, and running.

Old README info, to be removed when wiki construction is finished:

This app can be configured as any other CFS app would in a mission.  The *.tvm files should be placed in the 'fsw/tvm_files/' directory and all CFS data types w/ an associated mapping in a *.tvm file must have their type definition in a header file in the 'fsw/types_inc' directory.  However, it's a common-place for a mission to have a single 'include' directory somewhere that contains header files with all of the message definitions shared by CFS apps for that mission.  In this case, you can simply replace the 'fsw/types_inc' with a symbolic link to said directory and all should be well.