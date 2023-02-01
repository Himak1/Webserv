<?php include("templates/top.php");?>
	
		<div id="middle">
			<h3 class="">TELNET</h3>
			The Telnet syntax for testing open ports is:
			</br>telnet < address > < port number >
			</br>
			</br>for example:
			</br>telnet 127.0.0.1 4242
			</br>
			</br>After running the command, one of the following three options happen:
			</br>1. The command throws an error, indicating the port is not available for connection
			</br>2. The command goes to a blank screen, indicating the port is available.
			</br>3. Running the command on an open port 23 displays the screen of the telnet host, confirming an established Telnet connection
		</div>

<?php include("templates/bottom.php");?>
