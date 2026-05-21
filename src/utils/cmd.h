#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

int cmd_fork_exec(const char *cmd);
int cmd_send(const char *cmd);
int cmd_format_send(const char *format, const char *find, const char *replace);
char *cmd_output(const char *cmd);

G_END_DECLS
