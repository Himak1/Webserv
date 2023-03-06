<?php include("templates/top.php");?>

		<div id="middle">
			<h3 class="">FORM GET</h3>
			<form class="form" action = "form_handler.php" method="GET" enctype="multipart/form-data">
				<input class="input_field" placeholder="First Name" type = "text" name = "first_name"/>
				<input class="input_field" placeholder="Last Name" type = "text" name = "last_name"/>
				<input class="submit_button" type = "submit" value = "Submit" />
			</form>
		</div>

<?php include("templates/bottom.php");?>
