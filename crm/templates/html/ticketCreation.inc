<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html>
<html>
<?php
	$this->layout = 'full-width';
	$dir = dirname(__FILE__);
	include $dir.'/partials/header.inc';
?>
<body>
	<?php
		include $dir.'/partials/banner.inc';
		include $dir.'/partials/menubar.inc';
	?>

	<div id="panel-container">
		<div id="content-panel">
			<div id="ticketCreation" class="yui3-skin-sam">
				<?php include FRAMEWORK.'/errorMessages.php'; ?>
				<div id="location-panel">
					<?php echo $this->includeBlocks('location-panel'); ?>
				</div>
				<div id="person-panel">
					<?php echo $this->includeBlocks('person-panel'); ?>
				</div>
				<div id="ticket-panel">
					<?php echo $this->includeBlocks('ticket-panel'); ?>
				</div>
			</div>
		</div>
	</div>
	<?php include $dir.'/partials/footer.inc'; ?>
</body>
</html>
<?php
	include $dir.'/partials/garbageCollection.inc';
?>