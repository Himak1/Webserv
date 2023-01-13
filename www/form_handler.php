<?php include("templates/top.php");?>

		<div id="middle">
			<h3 class="">FORM</h3>
			<?php
				$first_name = getenv("first_name");
				$last_name = getenv("last_name");
				if($first_name !== NULL && $last_name !== NULL) {
					echo "<table><tr><td style='width:157px;'><h4>First Name</h4></td><td>";
					echo $first_name;
					echo "</td></tr><tr><td><h4>Last Name</h4></td><td>";
					echo $last_name. "</td></tr></table>";
				}
				else
					echo "Error: Invalid form data";
			?>
		</div>

<?php include("templates/bottom.php");?>
