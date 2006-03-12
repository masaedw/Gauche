/*
 * paths.c - get 'known' pathnames, such as the system's library directory.
 *
 *   Copyright (c) 2005 Shiro Kawai, All rights reserved.
 * 
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 * 
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  $Id: paths.c,v 1.1 2005-10-24 01:37:21 shirok Exp $
 *
 */

/* This file is used by both libgauche and gauche-config.  The latter
 * doesn't use ScmObj, so the function works on bare C strings.
 */

#include <string.h>
#include "gauche/config.h"
#include "gauche/arch.h"

#if defined(__MINGW32__)
#include "getdir_win.c"
#elif defined(GAUCHE_MACOSX_FRAMEWORK)
#include "getdir_darwin.c"
#else
#include "getdir_dummy.c"
#endif

/* The configure-generated path macros might have '@' at the beginning,
   indicating the runtime directory. */
void maybe_prepend_install_dir(const char *orig, char *buf, int buflen,
                               void (*errfn)(const char *, ...))
{
    if (*orig == '@') {
        int len = get_install_dir(buf, buflen, errfn);
        if (len + strlen(orig) > buflen) {
            errfn("Pathname too long: %s", orig);
        }
        strcat(buf, orig+1);
    } else {
        if (strlen(orig) >= buflen-1) {
            errfn("Pathname too long: %s", orig);
        }
        strcpy(buf, orig);
    }
}

/* External API */

void Scm_GetLibraryDirectory(char *buf, int buflen,
                             void (*errfn)(const char *, ...))
{
    maybe_prepend_install_dir(GAUCHE_LIB_DIR, buf, buflen, errfn);
}

void Scm_GetArchitectureDirectory(char *buf, int buflen,
                                  void (*errfn)(const char *, ...))
{
    maybe_prepend_install_dir(GAUCHE_ARCH_DIR, buf, buflen, errfn);
}

void Scm_GetSiteLibraryDirectory(char *buf, int buflen,
                                 void (*errfn)(const char *, ...))
{
    maybe_prepend_install_dir(GAUCHE_SITE_LIB_DIR, buf, buflen, errfn);
}

void Scm_GetSiteArchitectureDirectory(char *buf, int buflen,
                                      void (*errfn)(const char *, ...))
{
    maybe_prepend_install_dir(GAUCHE_SITE_ARCH_DIR, buf, buflen, errfn);
}
