OPTIONCCFLAGS = ""

if ARGUMENTS.get("debug", 0):
    OPTIONCCFLAGS += "-g"

SharedLibrary("chipmunk", Glob("*.c"), CPPPATH=["."], LIBS=["lua5.1", "chipmunk"], CCFLAGS=OPTIONCCFLAGS)
#Command("chipmunk.so", "libchipmunk.so",
#[
#Copy("tempfile", "$SOURCE"),
#Move("$TARGET", "tempfile"),
#])

