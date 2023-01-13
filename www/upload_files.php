<?php include("templates/top.php");?>

		<div id="middle">
			<h3 class="">DIRECTORY LIST</h3>
		<?php
			// $env_array =getenv();
			// foreach ($env_array as $key=>$value)
			// 	echo "$key => $value <br />";
			
			$directory_listing = getenv("directory_listing");
			if ($directory_listing !== "true") {
				echo "Directory listing is turned off";
				return;
			}

			$directory = getenv("upload_directory");
			if (!is_dir($directory)) {
				echo "Invalid upload directory";
				return;
			}

			if ($opendirectory = opendir($directory)) {
				while (($file = readdir($opendirectory)) !== false) {
					if ($file[0] != ".")
						echo "<a href='/$directory" . "$file'>$file</a>"."<br>";
				}
				closedir($opendirectory);
			}
		?>
		</div>

<?php include("templates/bottom.php");?>

