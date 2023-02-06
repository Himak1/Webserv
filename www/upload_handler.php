<?php include("templates/top.php");?>
		<div id="middle">
			<h3 class="">UPLOAD</h3>
			<?php
				$file_to_upload = getenv("file_to_upload");
				$upload_succes = getenv("upload_succes");
				if($file_to_upload !== NULL && $upload_succes == "true")
					echo "file succesfully uploaded!";
				else
					echo "Upload failed";
			?>
		</div>
<?php include("templates/bottom.php");?>
