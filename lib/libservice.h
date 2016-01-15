/*
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <libconfig.h>

#include <config.h>
#include <locale.h>
#include "gettext.h"

#define _(string) gettext(string)

// ...
int init_network(void);
