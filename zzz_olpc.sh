case "$LANG" in
*.utf-8|*.UTF-8)
	case "`tty`" in
	not\ a\ tty)
		# Don't print anything on non interactive sessions
	;;
	*)
		cat /etc/motd.olpc 2>/dev/null
	;;
	esac
;;
esac

# Add back sbin dirs to unprivileged users PATH
case $PATH in
	*/sbin/*) ;;
	*) PATH=/usr/local/sbin:/usr/sbin:/sbin:$PATH ;;
esac
