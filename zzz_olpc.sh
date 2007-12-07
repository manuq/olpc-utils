case "`tty`" in
	not\ a\ tty)
		# Don't print anything on non interactive sessions
		exit 0
	;;
	# Not needed: /etc/profile.d/lang.sh does it for us
	#tty[0-9])
	#	# The linux console forgets how to output utf-8 on each logoff
	#	/bin/unicode_start sun12x22
	#;;
	*)
		# No kludges needed for remote terminals
	;;
esac

case "$LANG" in
	*.utf-8|*.UTF-8)
		cat /etc/motd.olpc
	;;
esac
