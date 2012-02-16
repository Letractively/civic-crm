<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html>
<html>
<?php
	$this->layout = 'three-column';
	$dir = dirname(__FILE__);
	include $dir.'/partials/header.inc';
?>
<body>
	<?php
		include $dir.'/partials/banner.inc';
		include $dir.'/partials/menubar.inc';
	?>

	<div id="panel-container">
		<?php
			include $dir.'/partials/panel-one.inc';
			include $dir.'/partials/panel-two.inc';
		?>
		<div id="content-panel">
			<?php
				include FRAMEWORK.'/errorMessages.php';

				echo $this->includeBlocks();
			?>
		</div>
	</div>
	<?php include $dir.'/partials/footer.inc'; ?>
</body>
</html>
<?php
	include $dir.'/partials/garbageCollection.inc';
?>