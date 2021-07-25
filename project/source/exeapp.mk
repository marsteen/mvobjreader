#
# include this file into makefile with
# "include exeapp.mk"

ifeq ($(MSYSTEM), MINGW32)
EXEAPP = .exe
endif
