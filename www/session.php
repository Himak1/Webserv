<?php include("templates/top.php");?>
		<div id="middle">
			<h3>SESSION MANAGEMENT</h3>

			<?php
				// https://www.tutorialspoint.com/php/php_function_session_start.htm

				// session_start();

				// if ($_SERVER["REQUEST_METHOD"] == "POST")
				// 	$username = $_POST["username"];
				// else
				// 	$username = "";

				// if ($username)
				// 	$_SESSION["username"] = $username;

				// if (!isset($_SESSION["username"]))
				// 	echo 'You are not logged in.</h1>';
				// else
				// 	echo $_SESSION["username"]. 'is logged in.';
			?>

			<div style="<?php if (isset($_SESSION["username"])) echo "visibility: hidden;"; ?>">
				<form method="POST" class="form" action = "session.php">
					<input class="input_field" placeholder="Username" type = "text" name = "username" required/>
					<input class="input_field" placeholder="Password" type = "password" name = "password" required/>
					<input class="submit_button" type = "submit" value = "Login" />
				</form>
			</div>

			<div style="<?php if (!isset($_SESSION["username"])) echo "display: none;";?>">
				<a class="button_link" href="delete.php" role="button">Logout</a>
			</div>
		</div>

<?php include("templates/bottom.php");?>	