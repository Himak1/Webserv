<?php include("templates/top.php");?>		

		<div id="middle">
			<h3 label for="fileInput">SELECT A FILE TO UPLOAD</h3>
			<!-- <form method="POST" class="form" action="/upload.php" enctype="multipart/form-data">
				<input type="file" name="fileToUpload" id="fileToUpload">
				<input class="submit_button" type="submit" value="Upload" name="submit">
			</form> -->
			<!-- <form enctype="multipart/form-data" action="/upload_handler.php" method="POST" class="form"> -->

			<!-- <form enctype="multipart/form-data" action="/upload_handler.php" method="POST" class="form">
	            <input type="hidden" name="MAX_FILE_SIZE" value="30000" />
				<input type="file" name="file_to_upload" required/>
				<input class="submit_button" type = "submit" value="Upload" />
			</form> -->

			<form enctype="multipart/form-data" action="/upload_handler.php" method="POST" class="form"> 
				<input type="hidden" name="MAX_FILE_SIZE" value="30000" />
				<input type="file" name="file_to_upload" id="file_to_upload"/>
				<input class="submit_button" type = "submit" value = "Upload" />
			</form>
		</form>
		</div>

<?php include("templates/bottom.php");?>