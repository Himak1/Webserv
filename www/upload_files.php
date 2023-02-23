<?php include("templates/top.php");?>

		<div id="middle">
			<h3 class="">DIRECTORY LIST</h3>
		<?php
			
			$directory_listing = getenv("directory_listing");
			if ($directory_listing !== "true") {
				echo "Directory listing is turned off";
				return;
			}
			$directory = getenv("upload_directory");
			// echo "DIRECTORY = ". $directory . "<br>";
			if (!is_dir($directory)) {
				echo "You haven't uploaded any files";
				return;
			}

			if ($opendirectory = opendir($directory)) {
				while (($file = readdir($opendirectory)) !== false) {
					if ($file[0] != ".")
						echo "<a href='/$directory/" . "$file'>$file</a>"."<br>";
				}
				closedir($opendirectory);
			}
		?>
		</div>

<?php include("templates/bottom.php");?>
