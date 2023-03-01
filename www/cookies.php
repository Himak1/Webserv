<?php include("templates/top.php");?>
		<div id="middle">

			<h3>COOKIES</h3>

			<?php
				$cookie = getenv("cookie_value");
				if ($cookie)
					echo "Cookie is set to : ". $cookie;
			?>
			<a class="button_link" style="<?php if (!$cookie) echo "display: none;";?>" href="cookies_delete.php" role="button">Delete Cookie</a>

			<div style="<?php if ($cookie) echo "display: none;"; ?>">
				<form method="POST" class="form" action = "cookies.php"  enctype="multipart/form-data">
					<input class="input_field" placeholder="Cookie Value" type="text" name="cookie_value" required/>
					<input class="submit_button" type = "submit" value = "Store Cookie" />
				</form>
			</div>
		</div>

<?php include("templates/bottom.php");?>
