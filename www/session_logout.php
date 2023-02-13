<?php
	// if (isset($_COOKIE["cookie"])) {
	// 	unset($_COOKIE["cookie"]);
	// 	setcookie("cookie", null, -1, '/');
	// }
	// header( "refresh:0; url=index.php" );
	// $cookie = getenv("sessionID");
	// if ($cookie !== NULL) {
	// 	unset($cookie);
	// 	setcookie("sessionID", "", -1, '/');
	// }
?>
<?php include("templates/top.php");?>
		<div id="middle">
			<h3>SESSION MANAGEMENT</h3>
			You have been logged out.
		</div>
<?php include("templates/bottom.php");?>	