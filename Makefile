# The following variables need to be set prior to including library.mk

# Versioning
include ver/version.mk

# Library name
NAME = tokenizer

# If the project is separated among multiple sub-folders
PROJECT_ROOTS =

# Additionnal libraries (ex: -pthread, -lmath, etc)
LIBRARIES = 

# Additionnal flags for the compiler
FLAGS = -DMEMORY_WATCH

# Include the template
include $(CUT_HOME)CUT/res/library.mk
