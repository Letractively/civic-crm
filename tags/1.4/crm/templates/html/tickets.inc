<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
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
			<?php include FRAMEWORK.'/errorMessages.php'; ?>

			<div id="ticket-panel">
				<?php echo $this->includeBlocks('ticket-panel'); ?>
			</div>
			<div id="action-panel">
				<?php echo $this->includeBlocks('history-panel'); ?>
			</div>
			<div id="issue-panel">
				<?php echo $this->includeBlocks('issue-panel'); ?>
			</div>
			<div id="location-panel">
				<?php echo $this->includeBlocks('location-panel'); ?>
			</div>
		</div>
	</div>
	<?php
		include $dir.'/partials/footer.inc';
	?>
</body>
</html>
<?php
	include $dir.'/partials/garbageCollection.inc';
?>