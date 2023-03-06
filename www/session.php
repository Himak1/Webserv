<?php include("templates/top.php");?>
		<div id="middle">
			<h3>SESSION MANAGEMENT</h3>

			<?php
				$sessionID = getenv("sessionID");
				if ($sessionID)
					echo "<br>". $sessionID. ' is logged in.';
			?>

			<div style="<?php if ($sessionID) echo "visibility: hidden;"; ?>">
				<form method="POST" class="form" action = "session_login.php" enctype="multipart/form-data">
					<input class="input_field" placeholder="Username" type = "text" name = "username" required/>
					<input class="input_field" placeholder="Password" type = "password" name = "password" required/>
					<input class="submit_button" type = "submit" value = "Login" />
				</form>
			</div>

			<div style="<?php if (!$sessionID) echo "display: none;";?>">
				<a class="button_link" href="session_logout.php" role="button">Logout</a><br>
			</div>
		</div>

<?php include("templates/bottom.php");?>	
