<?php include("templates/top.php");?>
	
		<div id="middle">
			<h3 class="">GET</h3>
			curl -v -X GET "127.0.0.1:4242/"</br></br>
			curl -v -X GET "127.0.0.1:4242/form_handler.php?first_name=CODAM\&last_name=42"</br>
	
			</br><h3 class="">POST</h3>
			curl -v -X POST "127.0.0.1:4242/form_handler.php?first_name=CODAM\&last_name=42"</br>

			</br><h3 class="">DELETE</h3>
			curl -v -X DELETE "127.0.0.1:4242/uploads/Makefile"</br>

			</br><h3 class="">UNKNOWN</h3>
			curl -v -X HEAD "127.0.0.1:4242"</br>

			</br><h3 class="">UPLOAD FILES</h3>
			curl -v -X GET "127.0.0.1:4242/upload_handler.php?file_to_upload=Makefile"</br></br>
			curl -v -X GET "127.0.0.1:4242/upload_handler.php?file_to_upload=/Users/mweitenb/Desktop/test.txt"
		</div>

<?php include("templates/bottom.php");?>
