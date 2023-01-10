<?php include("templates/top.php");?>
		<div id="middle">
			<?php
				echo "files: "; var_dump($_FILES);
				echo "<br>post: "; var_dump($_POST);
				echo "<br>server: "; var_dump($_SERVER);
				echo "<br>request: "; var_dump($_REQUEST);
			?>
		</div>
<?php include("templates/bottom.php");?>
