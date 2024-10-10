### Almost universal Makefile ###
#This version is made for Linux
#>>> To compile on Windows change SYSTEM to WIN
SYSTEM = LINUX
# <make> to compile
# <make compile> to compile with bear and generate .json file for clangd!
# <make clean> to clean .d/.o files
# <make doxygen> to 

#====================================================================
#====================================================================

#compilier
#>>> PASS CC=<your compilier>
ifeq ($(origin CC),default)
	CC=clang++
endif

#Name of compiled executable
NAME = program.elf
#Name of directory where .o and .d files will be stored
OBJDIR = build
#Name of directory with headers
INCLUDEDIR = include
#Name of directory with .cpp
SRCDIR = src
#Name of directory where doxygen documentation will be generated
DOXYDIR = doxDocs
#Name of json file to store info about genetarions for clangd
COMPILEJSON = compile_commands.json

CFLAGS_WIN =	-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal						\
				-Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs						\
				-Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions 				\
				-Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 			\
				-Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor 			\
				-Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing 			\
				-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

CFLAGS_LINUX = 	-D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations 			\
				-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts 				\
				-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal 			\
				-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op 		\
				-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self 		\
				-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel 				\
				-Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types 	\
				-Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code 		\
				-Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers 	\
				-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new 					\
				-fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging 				\
				-fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla 			\
				-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,$\
				integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,$\
				returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

#flag to tell compiler where headers are located
override CFLAGS += -I./$(INCLUDEDIR)

CMD_DEL_LINUX = @rm -rf ./$(OBJDIR)/*.o ./$(OBJDIR)/*.d ./$(OBJDIR)/*.json
CMD_DEL_WIN   = @del .\$(OBJDIR)\*.o .\$(OBJDIR)\*.d .\$(OBJDIR)\*.json
CMD_MKDIR_LINUX = @mkdir -p $(OBJDIR)
CMD_MKDIR_WIN = @IF exist "$(OBJDIR)/" ( echo "" ) ELSE ( mkdir "$(OBJDIR)/" )

ifeq ($(SYSTEM),LINUX)
	CMD_DEL = $(CMD_DEL_LINUX)
	CMD_MKDIR = $(CMD_MKDIR_LINUX)
	CFLAGS += $(CFLAGS_LINUX)
else
	CMD_DEL = $(CMD_DEL_WIN)
	CMD_MKDIR = $(CMD_MKDIR_WIN)
	CFLAGS += $(CFLAGS_WIN)
endif

#Note: ALL cpps in source dir will be compiled
#Getting all cpps
SRCS := $(wildcard $(SRCDIR)/*.cpp)
#Replacing .cpp with .o, temporary variable
TOBJS := $(SRCS:%.cpp=%.o)
#Replacing src dir to obj dir
OBJS := $(TOBJS:$(SRCDIR)%=$(OBJDIR)%)
#Dependencies for .cpp files, they are stored with .o objects
DEPS := $(OBJS:%.o=%.d)


#Main target to compile executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

#Automatic target to compile object files
$(OBJS) : $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CMD_MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Idk how it works, but is uses compiler preprocessor to automatically generate
#.d files with included headears that make can use
$(DEPS) : $(OBJDIR)/%.d : $(SRCDIR)/%.cpp
	$(CMD_MKDIR)
	$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@


#====================================================================
#====================================================================
.PHONY:compile
compile:
	bear --output ./$(OBJDIR)/$(COMPILEJSON) -- make

.PHONY:init
init:
	$(CMD_MKDIR)

#Deletes all object and .d files
.PHONY:clean
clean:
	$(CMD_DEL)

.PHONY:doxygen
doxygen:
ifeq ($(SYSTEM), LINUX)
	@mkdir -p $(DOXYDIR)
else
	@IF exist "$(DOXYDIR)/" ( echo "" ) ELSE ( mkdir "$(DOXYDIR)/" )
endif
	doxygen Doxyfile

#====================================================================
#====================================================================

NODEPS = clean

#Includes make dependencies
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(DEPS)
endif