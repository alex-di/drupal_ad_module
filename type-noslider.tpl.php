<div <?php print drupal_attributes($attr); ?>>
        <ul>
            <?php foreach ($items AS $item): ?>
                <?php print $item; ?>
            <?php endforeach; ?>
        </ul>
</div>