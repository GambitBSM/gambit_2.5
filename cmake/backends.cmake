# GAMBIT: Global and Modular BSM Inference Tool  
#************************************************
# \file                                          
#                                                
#  Cmake configuration scripts for obtaining,
#  configuring, compiling and installing 
#  backends.
#  
#  Note that this is not necessarily the canonical
#  way to manage the compilation of all backends,
#  and GAMBIT support for backend compilation is
#  minimal, even with this method -- so please
#  contact the authors of the respective codes
#  if they won't compile!  
#    
#************************************************
#                                                
#  Authors (add name and date if you modify):
#
#  \author Antje Putze
#          (antje.putze@lapth.cnrs.fr)              
#  \date 2014 Sep, Oct, Nov
#
#  \author Pat Scott
#          (p.scott@imperial.ac.uk)              
#  \date 2014 Nov, Dec
#  \date 2015 May  
#
#  \author Chris Rogan
#          (crogan@cern.ch)              
#  \date 2015 May
#
#  \author Anders Kvellestad
#          (anderkve@fys.uio.no)
#  \date 2015 May
#                                               
#************************************************


# DarkSUSY
set(remove_files_from_libdarksusy dssetdsinstall.o dssetdsversion.o ddilog.o drkstp.o eisrs1.o tql2.o tred2.o)
set(remove_files_from_libisajet fa12.o  func_int.o  func.o  isalhd.o  isared.o)
set(DS_PATCH_DIR "${PROJECT_SOURCE_DIR}/Backends/patches/DarkSUSY/5.1.1")
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(_ld_prefix "-Wl,-all_load")
  set(_ld_suffix "")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(_ld_prefix "-Wl,--whole-archive")
  set(_ld_suffix "-Wl,--no-whole-archive")
endif()
set(libs ${_ld_prefix} <SOURCE_DIR>/lib/libFH.a <SOURCE_DIR>/lib/libHB.a <SOURCE_DIR>/lib/libdarksusy.a <SOURCE_DIR>/lib/libisajet.a ${_ld_suffix})
ExternalProject_Add(darksusy
  URL http://www.fysik.su.se/~edsjo/darksusy/tars/darksusy-5.1.1.tar.gz
  URL_MD5 ebeb0e1cfb4d834858e120190e423f62
  DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/Backends/downloaded
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/Backends/installed/DarkSUSY/5.1.1
  BUILD_IN_SOURCE 1
  PATCH_COMMAND patch -b -p1 -d src < ${DS_PATCH_DIR}//patchDS.dif 
        COMMAND patch -b -p1 -d contrib/isajet781-for-darksusy < ${DS_PATCH_DIR}/patchISA.dif
  CONFIGURE_COMMAND <SOURCE_DIR>/configure FC=${CMAKE_Fortran_COMPILER} FCFLAGS=${CMAKE_Fortran_FLAGS} FFLAGS=${CMAKE_Fortran_FLAGS} CC=${CMAKE_C_COMPILER} CFLAGS=${CMAKE_C_FLAGS} CXX=${CMAKE_CXX_COMPILER} CXXFLAGS=${CMAKE_CXX_FLAGS}
  BUILD_COMMAND make 
        COMMAND ar d <SOURCE_DIR>/lib/libdarksusy.a ${remove_files_from_libdarksusy} 
        COMMAND ar d <SOURCE_DIR>/lib/libisajet.a ${remove_files_from_libisajet}
  INSTALL_COMMAND ${CMAKE_Fortran_COMPILER} -shared ${libs} -o <SOURCE_DIR>/lib/libdarksusy.so 
)
set_property(TARGET darksusy PROPERTY _EP_DOWNLOAD_ALWAYS 0)
set(clean_files ${clean_files} "${PROJECT_SOURCE_DIR}/Backends/installed/DarkSUSY/5.1.1/lib/libdarksusy.so")

# SuperIso
ExternalProject_Add(superiso
  URL http://superiso.in2p3.fr/download/superiso_v3.4.tgz
  URL_MD5 ae4ecc45e7f608d9faf91ba8e5780053
  DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/Backends/downloaded
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/Backends/installed/SuperIso/3.4
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND sed ${dashi} "s#CC = gcc#CC = ${CMAKE_C_COMPILER}#g" <SOURCE_DIR>/Makefile 
        COMMAND sed ${dashi} "s/CFLAGS= -O3 -pipe -fomit-frame-pointer/CFLAGS= -lm -fPIC ${CMAKE_C_FLAGS}/g" <SOURCE_DIR>/Makefile
        COMMAND make 
        COMMAND ar x <SOURCE_DIR>/src/libisospin.a
        COMMAND echo "${CMAKE_C_COMPILER} -shared -o libsuperiso.so *.o -lm" > make_so.sh
        COMMAND chmod u+x make_so.sh
        COMMAND ./make_so.sh
  INSTALL_COMMAND ""
)
set_property(TARGET superiso PROPERTY _EP_DOWNLOAD_ALWAYS 0)
set(clean_files ${clean_files} "${PROJECT_SOURCE_DIR}/Backends/installed/SuperIso/3.4/libsuperiso.so")

# DDCalc
set(ddcalc_location "${GAMBIT_INTERNAL}/DDCalc0")
set(ddcalc_dir "${PROJECT_SOURCE_DIR}/Backends/installed/DDCalc/0.0")
ExternalProject_Add(ddcalc
  DOWNLOAD_COMMAND cmake -E echo "       Retrieving unreleased code from GAMBIT Collaboration private repository (will fail if you don't have the repository)."
           COMMAND cmake -E copy_directory ${ddcalc_location} ${ddcalc_dir}
  SOURCE_DIR ${ddcalc_dir}
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make libDDCalc0.so FC=${CMAKE_Fortran_COMPILER} FFLAGS=${CMAKE_Fortran_FLAGS} OUTPUT_PIPE=>/dev/null
  INSTALL_COMMAND ""
)
set(clean_files ${clean_files} "${ddcalc_dir}/libDDCalc0.so")

# Gamlike
if(GSL_FOUND)
  execute_process(
    COMMAND gsl-config --libs
    OUTPUT_VARIABLE GAMLIKE_GSL_LIBS
    RESULT_VARIABLE RET
  )
  if( RET EQUAL 0 )
    string( STRIP "${GAMLIKE_GSL_LIBS}" GAMLIKE_GSL_LIBS )
  endif()
endif()
set(gamlike_CXXFLAGS "${CMAKE_CXX_FLAGS}")
if (NOT GSL_INCLUDE_DIRS STREQUAL "")
  set(gamlike_CXXFLAGS "${gamlike_CXXFLAGS} -I${GSL_INCLUDE_DIRS}")
endif()
set(gamlike_location "${GAMBIT_INTERNAL}/gamLike")
set(gamlike_dir "${PROJECT_SOURCE_DIR}/Backends/installed/gamLike/1.0.0")
ExternalProject_Add(gamlike
  DOWNLOAD_COMMAND cmake -E echo "       Retrieving unreleased code from GAMBIT Collaboration private repository (will fail if you don't have the repository)."
           COMMAND cmake -E copy_directory ${gamlike_location} ${gamlike_dir}
  SOURCE_DIR ${gamlike_dir}
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make CXX=${CMAKE_CXX_COMPILER} CXXFLAGS=${gamlike_CXXFLAGS} LDFLAGS=${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} LDLIBS=${GAMLIKE_GSL_LIBS}
  INSTALL_COMMAND ""
)
set(clean_files ${clean_files} "${gamlike_dir}/gamLike.so")

# MicrOmegas for MSSM
ExternalProject_Add(micromegas
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/Backends/patches/micromegas/3.5.5/MSSM
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ./install_micromegas.script FC=${CMAKE_Fortran_COMPILER}
  INSTALL_COMMAND ""
)
set(clean_files ${clean_files} "${PROJECT_SOURCE_DIR}/Backends/installed/micromegas/3.5.5/MSSM/MSSM/libmicromegas.so")

# MicrOmegas for SingletDM
ExternalProject_Add(micromegasSingletDM
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/Backends/patches/micromegas/3.5.5/SingletDM
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ./install_micromegas.script FC=${CMAKE_Fortran_COMPILER}
  INSTALL_COMMAND ""
)
set(clean_files ${clean_files} "${PROJECT_SOURCE_DIR}/Backends/installed/micromegas/3.5.5/SingletDM/SingletDM/libmicromegas.so")

# Pythia
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
  set(pythia_CONFIGURE_EXTRAS "--enable-64bits")
endif()
# - Pythia will not accept the -std=c++11 flag. Create a special pythia_CXXFLAGS variable without it. 
string(REGEX REPLACE "(-std=c\\+\\+11)" "" pythia_CXXFLAGS ${CMAKE_CXX_FLAGS})
# - Suppress warnings from -Wextra when building Pythia with gcc
if("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "GNU") 
  set(pythia_CXXFLAGS "${pythia_CXXFLAGS} -Wno-extra")
endif()                                                 
# - Set include directories
set(pythia_CXXFLAGS "${pythia_CXXFLAGS} -I${Boost_INCLUDE_DIR} -I${PROJECT_SOURCE_DIR}/contrib/slhaea/include")
# - Set local paths
set(pythia_location "${GAMBIT_INTERNAL}/boss/bossed_pythia_source")
set(pythia_dir "${PROJECT_SOURCE_DIR}/Backends/installed/Pythia/8.209")
# - Actual configure and compile commands
ExternalProject_Add(pythia
  DOWNLOAD_COMMAND cmake -E echo "       Retrieving unreleased code from GAMBIT Collaboration private repository (will fail if you don't have the repository)."
           COMMAND cmake -E copy_directory ${pythia_location} ${pythia_dir}
  SOURCE_DIR ${pythia_dir}
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ./configure --enable-shared --cxx="${CMAKE_CXX_COMPILER}" --cxx-common="${pythia_CXXFLAGS}" --cxx-shared="${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS}" --lib-suffix=".so" ${pythia_CONFIGURE_EXTRAS}
  COMMAND echo "OSX DEBUG: CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}"
  COMMAND echo "OSX DEBUG: pythia_CXXFLAGS = ${pythia_CXXFLAGS}"
  COMMAND echo "OSX DEBUG: pythia_CONFIGURE_EXTRAS = ${pythia_CONFIGURE_EXTRAS}"
  COMMAND echo "OSX DEBUG: CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS = ${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS}"
  BUILD_COMMAND make CXX="${CMAKE_CXX_COMPILER}"
  INSTALL_COMMAND ""
)
set(clean_files ${clean_files} "${pythia_dir}/config.mk" "${pythia_dir}/lib/libpythia8.so")

# Fastsim
set(fastsim_location "${GAMBIT_INTERNAL}/fast_sim")
set(fastsim_dir "${PROJECT_SOURCE_DIR}/Backends/installed/fastsim/1.0")
ExternalProject_Add(fastsim
  DOWNLOAD_COMMAND cmake -E echo "       Retrieving unreleased code from GAMBIT Collaboration private repository (will fail if you don't have the repository)."
           COMMAND cmake -E copy_directory ${fastsim_location} ${fastsim_dir}
  SOURCE_DIR ${fastsim_dir}
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make CXX=${CMAKE_CXX_COMPILER} CXXFLAGS=${CMAKE_CXX_FLAGS} LDFLAGS=${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} libfastsim.so
  INSTALL_COMMAND cp ""
)
set(clean_files ${clean_files} "${fastsim_dir}/lib/libfastsim.so")

# Nulike
set(nulike_location "${GAMBIT_INTERNAL}/nulike")
set(nulike_ver "1\\.0\\.0")
set(nulike_lib "libnulike")
set(nulike_dir "${PROJECT_SOURCE_DIR}/Backends/installed/nulike/1.0.0")
set(nulike_short_dir "./Backends/installed/nulike/1.0.0")
set(nulikeFFLAGS "${CMAKE_Fortran_FLAGS} -I${nulike_dir}/include")
ExternalProject_Add(nulike
  #URL 
  #URL_MD5 
  #DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/Backends/downloaded
  DOWNLOAD_COMMAND cmake -E echo "       Retrieving unreleased code from GAMBIT Collaboration private repository (will fail if you don't have the repository)."
           COMMAND cmake -E copy_directory ${nulike_location} ${nulike_dir}
  SOURCE_DIR ${nulike_dir}
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make ${nulike_lib}.so FC=${CMAKE_Fortran_COMPILER} FFLAGS=${nulikeFFLAGS} MODULE=${FMODULE} 
  INSTALL_COMMAND sed ${dashi} "s#${nulike_ver}:.*${nulike_lib}\\.so#${nulike_ver}:       ${nulike_short_dir}/lib/${nulike_lib}.so#g" ${PROJECT_SOURCE_DIR}/config/backend_locations.yaml
)
set_property(TARGET nulike PROPERTY _EP_DOWNLOAD_ALWAYS 0)
set(clean_files ${clean_files} "${nulike_dir}/lib/${nulike_lib}.so")

# SUSY-HIT
set(susyhit_ver "1\\.5")
set(susyhit_lib "libsusyhit")
set(susyhit_dir "${PROJECT_SOURCE_DIR}/Backends/installed/SUSY-HIT/1.5")
set(susyhit_short_dir "./Backends/installed/SUSY-HIT/1.5")
ExternalProject_Add(susyhit
  URL http://www.itp.kit.edu/~maggie/SUSY-HIT/susyhit.tar.gz
  URL_MD5 493c7ba3a07e192918d3412875fb386a
  DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/Backends/downloaded
  SOURCE_DIR ${susyhit_dir}
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND cp -n makefile makefile.orig  COMMAND cp -n sdecay.f sdecay.orig  COMMAND cp -n hdecay.f hdecay.orig 
            COMMAND cp makefile.orig makefile.tmp COMMAND cp sdecay.orig sdecay.f.tmp COMMAND cp hdecay.orig hdecay.f.tmp
            COMMAND sed ${dashi} -e "s#FC=\\(.*\\)[[:space:]]*$#FF=\\1${nl}${nl}FC=\\$\\(FF\\)${nl}FFLAGS= -O2 -fPIC#g"
                                 -e "s#FC)[[:space:]]*-c#FC\\) \\$\\(FFLAGS\\) -c#g" 
                                 -e "s#\\(clean:.*\\)$#${susyhit_lib}.so:\\$\\(OBJS1\\) \\$\\(OBJS2\\) \\$\\(OBJS3\\)${nl}\t\\$\\(FC\\) -shared -o \\$@ \\$\\(OBJS1\\) \\$\\(OBJS2\\) \\$\\(OBJS3\\)${nl}${nl}\\1#g"
                                 makefile.tmp
            COMMAND sed ${dashi} -e "s#^[[:space:]]*program sdecay[[:space:]]*$#      !Added by GAMBIT to make 100% sure \\'unlikely\\' matches the fortran value.${nl}      double precision function s_hit_unlikely()${nl}        s_hit_unlikely = -123456789D0${nl}      end${nl}${nl}      subroutine sdecay                          !Modified by GAMBIT.#g"
                                 -e "s#COMMON/SD_stopwidth/stoptot4[[:space:]]*$#COMMON/SD_stopwidth/stoptot4,stoptot       !Modified by GAMBIT#g"
                                 -e "s#\\([[:space:]]*open(ninshs,file=.*\\)$#      if (.false.) then                          !Added by GAMBIT.${nl}${nl}\\1#g"
                                 -e "s#\\(^[[:space:]]*flagshsin=2d0\\)$#\\1${nl}      endif                                      !Added by GAMBIT.${nl}${nl}      else                                       !Added by GAMBIT.${nl}${nl}        flagshsin=0d0                            !Added by GAMBIT.${nl}        flagoutput=2d0                           !Added by GAMBIT.${nl}        i4bod=1                                  !Added by GAMBIT.${nl}#g"
                                 -e "s#\\([[:space:]]*if((flagoutput\\.eq\\.1\\.D0).or.(ifavvio\\.eq\\.1)) then\\)#c\\1 !Commented out by GAMBIT.#g"
                                 -e "s#\\!\\([[:space:]]*if(flagoutput\\.eq\\.1\\.D0) then\\)[[:space:]]*$#\\1                !Reinstated by GAMBIT.#g"
                                 -e "/output not a la Les Houches accord/{" -e "N" -e "N" -e "s/else/elseif(flagoutput.eq.0.D0) then            !Modified by GAMBIT./" -e "}"
                                 -e "s#\\(^[[:space:]]*if(flagshsin\\.\\)eq\\(\\.1\\.D0) then\\)#\\1le\\2                 !Modified by GAMBIT.#g"
                                 sdecay.f.tmp
            COMMAND sed ${dashi} -e "/=1: READ SUSY LES HOUCHES ACCORD INPUT/{" -e "N" -e "N" -e "s/\\(.*${nl}c end change susyhit[[:space:]]\\)/\\1C           =2: SLHA INPUT PROVIDED BY CALLING PROGRAM  !Added by GAMBIT.${nl}/" -e "}"
                                 -e "/=1: WRITE SUSY LES HOUCHES ACCORD OUTPUT/{" -e "N" -e "N" -e "s/\\(.*${nl}c end change susyhit[[:space:]]\\)/\\1C           =2: WRITE NOTHING                           !Added by GAMBIT.${nl}/" -e "}"
                                 -e "s#[[:space:]]*if(islhao\\.ne\\.1) then[[:space:]]*\$#      if(islhao.eq.0) then                       !Modified by GAMBIT.#g"
                                 -e "s#[[:space:]]*\\(CALL CLOSE_HDEC\\)[[:space:]]*\$#      if(islhao.eq.0) \\1            !Modified by GAMBIT.#g"
                                 -e "s#[[:space:]]*islhai[[:space:]]*=[[:space:]]*1[[:space:]]*\$#      islhai  = 2                                !Modified by GAMBIT.#g"
                                 -e "s#[[:space:]]*islhao[[:space:]]*=[[:space:]]*1[[:space:]]*\$#      islhao  = 2                                !Modified by GAMBIT.#g"
                                 -e "/[[:space:]]*if(islhai\\.eq\\.1)[[:space:]]*then[[:space:]]*\$/{" -e "N" -e "s#\\([[:space:]]*if(islhai\\.eq\\.1)[[:space:]]*then\\)#      if(islhai.ge.1) then                       !Added by GAMBIT.${nl}  \\1#" -e "}"
                                 -e "s#[[:space:]]*call SLHA_read_leshouches_HDEC(ninlha)#         call SLHA_read_leshouches_HDEC(ninlha)${nl}        endif                                    !Added by GAMBIT.#g"
                                 -e "s#\\(c -- calculation of the mb pole mass from mb(mb)_MSbar --[[:space:]]*\\)\$#      if(ishai.eq.2) fmb = massval(34)           !Added by GAMBIT.${nl}\\1#g"
                                 -e "/c -- calculation of the mb pole mass from mb(mb)_MSbar --[[:space:]]*\$/{" -e "N" -e "s#\\([[:space:]]*if(smval(5)\\.ne\\.0\\.D0\\))[[:space:]]*then[[:space:]]*\$#\\1 .and. ishai.ne.2) then !Modified by GAMBIT.#" -e "}"
                                 -e "/[[:space:]]*close(nout)[[:space:]]*\$/{" -e "N" -e "N" -e "s#else#elseif (islhao .eq. 0) then                !Modified by GAMBIT.#" -e "}"
                                 hdecay.f.tmp
            COMMAND awk "{gsub(/${nl}/,${true_nl})}{print}" makefile.tmp > makefile
            COMMAND awk "{gsub(/${nl}/,${true_nl})}{print}" sdecay.f.tmp > sdecay.f
            COMMAND awk "{gsub(/${nl}/,${true_nl})}{print}" hdecay.f.tmp > hdecay.f
            COMMAND cmake -E remove makefile.tmp
            COMMAND cmake -E remove sdecay.f.tmp
            COMMAND cmake -E remove hdecay.f.tmp
  BUILD_COMMAND make ${susyhit_lib}.so FC=${CMAKE_Fortran_COMPILER} FFLAGS=${CMAKE_Fortran_FLAGS}
  INSTALL_COMMAND sed ${dashi} "s#${susyhit_ver}:.*${susyhit_lib}\\.so#${susyhit_ver}:         ${susyhit_short_dir}/${susyhit_lib}.so#g" ${PROJECT_SOURCE_DIR}/config/backend_locations.yaml
)
set_property(TARGET susyhit PROPERTY _EP_DOWNLOAD_ALWAYS 0)
set(clean_files ${clean_files} "${susyhit_dir}/${susyhit_lib}.so")

# FeynHiggs
ExternalProject_Add(feynhiggs
  URL http://wwwth.mpp.mpg.de/members/heinemey/feynhiggs/newversion/FeynHiggs-2.10.4.tar.gz
  URL_MD5 afd04154870ab5519603ffdb0e4e2d5b
  DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/Backends/downloaded
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/Backends/installed/FeynHiggs/2.10.4
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND <SOURCE_DIR>/configure FC=${CMAKE_Fortran_COMPILER} FFLAGS=${CMAKE_Fortran_FLAGS} CC=${CMAKE_C_COMPILER} CFLAGS=${CMAKE_C_FLAGS} CXX=${CMAKE_CXX_COMPILER} CXXFLAGS=${CMAKE_CXX_FLAGS}
  BUILD_COMMAND make COMMAND mkdir -p lib COMMAND echo "${CMAKE_Fortran_COMPILER} -shared -o lib/libFH.so build/*.o" > make_so.sh COMMAND chmod u+x make_so.sh COMMAND ./make_so.sh
  INSTALL_COMMAND ""
)
set_property(TARGET feynhiggs PROPERTY _EP_DOWNLOAD_ALWAYS 0)
set(clean_files ${clean_files} "${PROJECT_SOURCE_DIR}/Backends/installed/FeynHiggs/2.10.4/lib/libFH.so")

# HiggsBounds
set(higgsbounds_tables_dir "${PROJECT_SOURCE_DIR}/Backends/installed/csboutput_trans_binary")
ExternalProject_Add(higgsbounds_tables
  URL http://www.hepforge.org/archive/higgsbounds/csboutput_trans_binary.tar.gz
  URL_MD5 004decca30335ddad95654a04dd034a6
  DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/Backends/downloaded
  SOURCE_DIR ${higgsbounds_tables_dir}
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)
set_property(TARGET higgsbounds_tables PROPERTY _EP_DOWNLOAD_ALWAYS 0)
ExternalProject_Add(higgsbounds
  DEPENDS higgsbounds_tables
  URL http://www.hepforge.org/archive/higgsbounds/HiggsBounds-4.2.0.tar.gz
  URL_MD5 9d76eefecea870d941a6fe8c0ee7a6ae
  DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/Backends/downloaded
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/Backends/installed/HiggsBounds/4.2.0
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND cp configure-with-chisq my_configure
            COMMAND sed ${dashi} -e "s|.*clsbtablesdir=.*|clsbtablesdir=\"${higgsbounds_tables_dir}\"|" <SOURCE_DIR>/my_configure
            COMMAND sed ${dashi} -e "s|F90C =.*|F90C = ${CMAKE_Fortran_COMPILER}|" <SOURCE_DIR>/my_configure
            COMMAND sed ${dashi} -e "s|F77C =.*|F77C = ${CMAKE_Fortran_COMPILER}|" <SOURCE_DIR>/my_configure
            COMMAND sed ${dashi} -e "s|F90FLAGS =.*|F90FLAGS = ${CMAKE_Fortran_FLAGS}|" <SOURCE_DIR>/my_configure
            COMMAND <SOURCE_DIR>/my_configure
  BUILD_COMMAND make COMMAND mkdir -p lib COMMAND echo "${CMAKE_Fortran_COMPILER} -shared -o lib/libhiggsbounds.so *.o" > make_so.sh COMMAND chmod u+x make_so.sh COMMAND ./make_so.sh
  INSTALL_COMMAND ""
)
set_property(TARGET higgsbounds PROPERTY _EP_DOWNLOAD_ALWAYS 0)
set(clean_files ${clean_files} "${PROJECT_SOURCE_DIR}/Backends/installed/HiggsBounds/4.2.0/lib/libhiggsbounds.so")

# HiggsSignals
ExternalProject_Add(higgssignals
  DEPENDS higgsbounds
  URL http://www.hepforge.org/archive/higgsbounds/HiggsSignals-1.3.2.tar.gz
  URL_MD5 2e300a3784eb5d3a9e1dd905d2af7676
  DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/Backends/downloaded
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/Backends/installed/HiggsSignals/1.3.2
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND cp configure my_configure 
            COMMAND sed ${dashi} -e "s|HBLIBS =.*|HBLIBS =-L../../HiggsBounds/4.2.0|" <SOURCE_DIR>/my_configure
            COMMAND sed ${dashi} -e "s|HBINCLUDE =.*|HBINCLUDE =-I../../HiggsBounds/4.2.0|" <SOURCE_DIR>/my_configure
            COMMAND sed ${dashi} -e "s|F90C =.*|F90C = ${CMAKE_Fortran_COMPILER}|" <SOURCE_DIR>/my_configure
            COMMAND sed ${dashi} -e "s|F77C =.*|F77C = ${CMAKE_Fortran_COMPILER}|" <SOURCE_DIR>/my_configure
            COMMAND sed ${dashi} -e "s|F90FLAGS =.*|F90FLAGS = ${CMAKE_Fortran_FLAGS}|" <SOURCE_DIR>/my_configure
            COMMAND <SOURCE_DIR>/my_configure
  BUILD_COMMAND make 
        COMMAND mkdir -p lib 
        COMMAND rm HiggsSignals.o 
        COMMAND echo "${CMAKE_Fortran_COMPILER} -shared -o lib/libhiggssignals.so ./*.o ../../HiggsBounds/4.2.0/*.o" > make_so.sh 
        COMMAND chmod u+x make_so.sh 
        COMMAND ./make_so.sh
  INSTALL_COMMAND ""
)
set_property(TARGET higgssignals PROPERTY _EP_DOWNLOAD_ALWAYS 0)
set(clean_files ${clean_files} "${PROJECT_SOURCE_DIR}/Backends/installed/HiggsSignals/1.3.2/lib/libhiggssignals.so")


set_target_properties(ddcalc gamlike darksusy micromegas micromegasSingletDM superiso nulike pythia fastsim  
                      higgssignals higgsbounds higgsbounds_tables feynhiggs susyhit PROPERTIES EXCLUDE_FROM_ALL 1)

add_custom_target(backends COMMAND make ddcalc gamlike darksusy micromegas micromegasSingletDM superiso nulike pythia  
                      higgssignals higgsbounds higgsbounds_tables feynhiggs susyhit) #fastsim
