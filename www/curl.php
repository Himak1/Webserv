<?php include("templates/top.php");?>
	
		<div id="middle">
			<h3 class="">GET</h3>
			curl -v GET 127.0.0.1:4242/</br>
			curl -v GET 127.0.0.1:4242/form_handler.php?first_name=CODAM\&last_name=42</br>
	
			</br><h3 class="">POST</h3>
			curl -v POST 127.0.0.1:4242/form_handler.php?first_name=CODAM\&last_name=42</br>

			</br><h3 class="">DELETE</h3>
			<!-- to do: werkt niet op windows via WSL terminal -->
			curl -v DELETE 127.0.0.1:4242/uploads/Makefile</br>

			</br><h3 class="">UNKNOWN</h3>
			curl -v HEAD 127.0.0.1:4242</br>

			</br><h3 class="">UPLOAD FILES</h3>
			curl -v GET 127.0.0.1:4242/upload_handler.php?file_to_upload=Makefile</br>
			
		</div>

<?php include("templates/bottom.php");?>
