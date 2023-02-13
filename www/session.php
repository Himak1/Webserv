<?php include("templates/top.php");?>
		<div id="middle">
			<h3>SESSION MANAGEMENT</h3>

			<?php
				// https://www.tutorialspoint.com/php/php_function_session_start.htm

				// session_start();

				// if ($_POST)
				// 	$username = $_POST["username"];
				// else
				// 	$username = "";

				// if ($username)
					// $_SESSION["username"] = $username;

				// echo "cookie = ". $cookie;
				// echo "<br>username = ". $username;

				// if (isset($_SESSION["username"]))
				// 	echo "<br>". $_SESSION["username"]. 'is logged in.';

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