<?php include("templates/top.php");?>
		<div id="middle">
			<h3>SESSION MANAGEMENT</h3>
			<?php
				$sessionID = getenv("sessionID");
				if ($sessionID)
					echo "<br>". $sessionID. 'is logged in.';
				else
					echo "<br> You are logged in.";
			?>
		</div>
<?php include("templates/bottom.php");?>	