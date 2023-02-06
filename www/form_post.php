<?php include("templates/top.php");?>

		<div id="middle">
			<h3 class="">FORM POST</h3>
			<form method="POST" class="form" action = "form_handler.php" enctype="multipart/form-data">
				<input class="input_field" placeholder="First Name" type = "text" name = "first_name" required/>
				<input class="input_field" placeholder="Last Name" type = "text" name = "last_name" required/>
				<input class="submit_button" type = "submit" value = "Submit" />
			</form>
		</div>

<?php include("templates/bottom.php");?>
