<?php include("templates/top.php");?>		

		<div id="middle">
			<h3 label for="fileInput">SELECT A FILE TO UPLOAD</h3>
			<form action="/upload_handler.php" method="GET" class="form"> 
				<input type="file" name="file_to_upload" id="file_to_upload"/>
				<input class="submit_button" type = "submit" value = "Upload" />
			</form>
		</form>
		</div>

<?php include("templates/bottom.php");?>