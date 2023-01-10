<?php include("templates/top.php");?>
		<div id="middle">

			<h3>COOKIES</h3>

			<?php
				$cookie_name = "cookie";
				if ($_SERVER["REQUEST_METHOD"] == "POST")
					$cookie_value = $_POST["cookie_value"];
				else
					$cookie_value = "";

				if ($cookie_value) {
					setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/");
					header("Refresh:0");
				}

				if (!isset($_COOKIE[$cookie_name]))
					echo 'Cookie was not set.';
				else
					echo 'Cookie: ' . $_COOKIE[$cookie_name];
			?>

			<h4>Set a cookie:</h4>
			<form method="POST" class="form" action = "cookies.php">
				<input class="input_field" placeholder="Cookie Name" type="text" id="cookie_name" required/>
				<input class="input_field" placeholder="Cookie Value" type="text" id="cookie_value" required/>
				<input class="submit_button" type = "submit" value = "Submit" />
			</form>
			<a class="button_link" href="cookies_delete.php" role="button">Delete Cookie</a>

			<!-- <form class="form-inline mx-auto" action="index.php" method="post"> -->
					<!-- <input type="text" readonly class="form-control-plaintext" id="cookie_name" value="Cookie Name"> -->
					<!-- <input type="text" name="cookie_value" class="form-control" id="cookie_value" placeholder="Cookie Value"> -->
				<!-- <button type="submit" class="btn btn-success mx-2">Set Cookie</button> -->
				<!-- <a class="btn btn-danger mx-2" href="delete.php" role="button">Delete Cookie</a> -->
			<!-- </form> -->
		</div>

<?php include("templates/bottom.php");?>
