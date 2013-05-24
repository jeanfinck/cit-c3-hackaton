<?php
// var_dump($user_profile['Personal Info']['profile_bio']['#markup']);exit;
?>

<div class="prof-blocks">
<h1><?php echo $user->name ?></h1>
<table>
	<tr>
		<td rowspan="2"><?php echo $user_profile['user_picture']['#markup'];?></td>
		<td align="right"><em class="labeling">bio</em></td>
		<td><small><?php echo $user_profile['Personal Info']['profile_bio']['#markup'];?></small></td>
	</tr>
<!-- 	<tr>
		<td><em class="labeling">status</em></td>
		<td>STATUS</td>
	</tr> -->
	</table>
</div>

<div class="prof-blocks">
<h2>Achievements</h2>
<ul>
	<li>
		<a href="#" class="tooltip">
			<img src="/sites/default/files/public/badges/badge-commenter.png" />
			<span>
				<strong>Commenter</strong>
				<br /> Comment at least 10 answers
			</span>
		</a>
	</li>
	<li>
		<a href="#" class="tooltip"> 
			<img src="/sites/default/files/public/badges/badge-interaction.png" />
			<span>
				<strong>First Interaction</strong>
				<br /> Make a new question or answer
			</span>
		</a>
	</li>
	<li class="inactive">
		<a href="#" class="tooltip">
			<img src="/sites/default/files/public/badges/badge-mastering.png" />
			<span>
				<strong>Technology Mastering</strong>
				<br /> Answer at least 10 questions in different categories
			</span>
		</a>
	</li>
	<li class="inactive">
		<a href="#" class="tooltip">
			<img src="/sites/default/files/public/badges/badge-contributor.png" />
			<span>
				<strong>Contributor Member</strong>
				<br /> Be a part of the team
			</span>
		</a>
	</li>
	<li class="inactive">
		<a href="#" class="tooltip"> <img src="/sites/default/files/public/badges/badge-wise.png" /><span>
				<strong>The Wise</strong>
				<br /> Answer at least 10 questions</span></a></li>
	<li>
		<a href="#" class="tooltip"> <img src="/sites/default/files/public/badges/badge-questioner.png" /><span>
				<strong>Questioner</strong>
				<br /> Make at least 10 questions</span></a></li>
	<li>
		<a href="#" class="tooltip"> <img src="/sites/default/files/public/badges/badge-favorite.png" /><span>
				<strong>The Favorite</strong>
				<br /> Receive at least 10 votes</span></a></li>
	<li class="inactive">
		<a href="#" class="tooltip"> <img src="/sites/default/files/public/badges/badge-achiever.png" /><span>
				<strong>Achiever</strong>
				<br /> Collect at least 7 badges</span></a></li>
</ul>
</div>
<br class="clear" />
<!-- <div class="prof-blocks">
<h2>Last 10 Questions</h2>
</div>
<div class="prof-blocks">
<h2>Last 10 Answers</h2>
</div> -->
<script>
	jQuery('#main-content-header').remove();
</script>
