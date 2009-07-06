#!/bin/sh
# From Martin Pitt at https://bugs.freedesktop.org/show_bug.cgi?id=14053
# Script to create /var/console/USER tag files compatible with pam_console
#
# Still needed as of dbus-1.2.12, which uses these files for the at_console
# check. You'd think it would use ConsoleKit to determine that, but it seems
# like they are not keen on dbus having to send a dbus message to figure it
# out.

TAGDIR=/var/run/console

[ -n "$CK_SESSION_USER_UID" ] || exit 1
[ "$CK_SESSION_IS_LOCAL" = "true" ] || exit 0

TAGFILE="$TAGDIR/`getent passwd $CK_SESSION_USER_UID | cut -f 1 -d:`"

if [ "$1" = "session_added" ]; then
    mkdir -p "$TAGDIR"
    echo "$CK_SESSION_ID" >> "$TAGFILE"
fi

if [ "$1" = "session_removed" ] && [ -e "$TAGFILE" ]; then
    sed -i "\%^$CK_SESSION_ID\$%d" "$TAGFILE"
    [ -s "$TAGFILE" ] || rm -f "$TAGFILE"
fi
