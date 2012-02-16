<?php
/**
 * @copyright 2011-2012 City of Bloomington, Indiana
 * @license http://www.gnu.org/licenses/agpl.txt GNU/AGPL, see LICENSE.txt
 * @author Cliff Ingham <inghamn@bloomington.in.gov>
 * @param TicketList $this->ticketList
 *
 * @param string $this->title
 * @param bool $this->disableButtons
 * @param Ticket $this->filterTicket A ticket to leave out of the display
 * @param int $this->limit
 * @param string $this->moreLink
 */
if ($this->limit) {
	$this->ticketList->limit($this->limit);
}
?>
<div class="ticketList">
	<h1><?php echo $this->title ? View::escape($this->title) : 'Cases'; ?></h1>
	<table>
		<thead>
			<tr><th>Status</th>
				<th>Location</th>
				<th>Entered Date</th>
				<?php if (userIsAllowed('people','view')) { echo "<th>Entered By</th>"; } ?>
				<th>Category</th>
				<th></th>
			</tr>
		</thead>
		<tbody>
		<?php
			foreach ($this->ticketList as $ticket) {
				if ($this->filterTicket
					&& "{$this->filterTicket->getId()}"=="{$ticket->getId()}") {
					continue;
				}
				$status = View::escape($ticket->getStatus());
				$location = View::escape($ticket->getLocation());
				$enteredDate = $ticket->getEnteredDate(DATE_FORMAT);
				$enteredByPerson = $ticket->getEnteredByPerson();
				$enteredBy = $enteredByPerson ? View::escape($enteredByPerson->getFullname()) : '';
				$category = View::escape($ticket->getData('category.name'));
				if (!$this->disableLinks) {
					$status = "<a href=\"{$ticket->getURL()}\">$status</a>";
					$location = "<a href=\"{$ticket->getURL()}\">$location</a>";
					$enteredDate = "<a href=\"{$ticket->getURL()}\">$enteredDate</a>";
					$enteredBy = "<a href=\"{$ticket->getURL()}\">$enteredBy</a>";
					$category = "<a href=\"{$ticket->getURL()}\">$category</a>";
				}
				echo "
				<tr><td>$status</td>
					<td>$location</td>
					<td>$enteredDate</td>
				";
					if (userIsAllowed('people','view')) { echo "<td>$enteredBy</td>"; }
				echo "
					<td>$category</td>
					<td>
				";
					if (!$this->disableButtons
							&& userIsAllowed('issues','update')
							&& $ticket->getStatus()!=='closed') {
						$url = new URL(BASE_URL.'/issues/update');
						$url->ticket_id = "{$ticket->getId()}";
						if (isset($_GET['person_id'])) {
							try {
								$person = new Person($_GET['person_id']);
								$url->person_id = "{$person->getId()}";
							}
							catch (Exception $e) {
								// Just ignore invalid people
							}
						}
						echo "<a class=\"add button\" href=\"$url\">Add Issue</a>";
					}
				echo "
					</td>
				</tr>
				";
			}
		?>
		</tbody>
	</table>
	<?php
		if ($this->limit && count($this->ticketList)>$this->limit && $this->moreLink) {
			echo "<div><a href=\"{$this->moreLink}\">More &gt;&gt;</a></div>";
		}
		if (userIsAllowed('tickets','merge') && !$this->disableLinks && count($this->ticketList)>1) {
			$this->template->addToAsset('scripts',YUI.'/yui/yui-min.js');
			$this->template->addToAsset('scripts',BASE_URL.'/js/tickets/merge.js');
			echo "
			<form method=\"get\" action=\"".BASE_URL."/tickets/merge\">
				<fieldset><legend>Merge Two Cases</legend>
					<p>Add two cases by dragging one of the rows or typing in the IDs to compare cases and move data between them.</p>
					<div>
						<span class=\"yui3-dd-drop\"><input name=\"ticket_id_a\" id=\"ticket_id_a\" /></span>
						<span class=\"yui3-dd-drop\"><input name=\"ticket_id_b\" id=\"ticket_id_b\" /><span class=\"yui3-dd-drop\">
						<button type=\"submit\" class=\"submit\">Submit</button>
					</div>
				</fieldset>
			</form>
			";
		}
	?>
</div>