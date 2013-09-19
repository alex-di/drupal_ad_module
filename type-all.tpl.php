<div <?php print drupal_attributes($attr); ?>>
    <div class="slider">
        <ul>
            <?php foreach ($items AS $item): ?>
                <li><?php print $item; ?></li>
            <?php endforeach; ?>
        </ul>
    </div>
    <div class="controls">
        <span class="control-item"><a href="#" class="prev-slide">&lt; Предыдущее</a></span>
        <?php if ($content): ?><span class="add-content control-item"><?php print $content; ?></span><?php endif; ?>
        <span class="control-item"><a href="#" class="next-slide">Следующее &gt;</a></span>
    </div>
</div>
<script>// slider autoplay function
    jQuery(document).ready(function() {
        jQuery('.ad-wrapper-<?php print $hash; ?> .slider script').remove();
        var $slider = jQuery('.ad-wrapper-<?php print $hash; ?> .slider');
        if ($slider.width() < $slider.find('li').width() * $slider.find('li').length) {
            $slider.lemmonSlider({'items': 'li', 'infinite': true});
            sliderAutoplay('<?php print $hash; ?>');
            if (!$slider.next().filter('.empty-block-2').length)
                $slider.after('<div class="empty-block-2"></div>');
        }
        else
            $slider.next().filter('.controls').addClass('invisible');
        jQuery(window).resize(function() {
            console.log($slider.attr('data-slider-infinite'));
            if (typeof $slider.prop('data-slider-infinite') === 'undefined' && ($slider.width() < $slider.find('li').width() * $slider.find('li').length)) {
                $slider.lemmonSlider({'items': 'li', 'infinite': true});
                sliderAutoplay('<?php print $hash; ?>');
                $slider.next().filter('.controls').removeClass('invisible');
                if (!$slider.next().filter('.empty-block-2').length)
                    $slider.after('<div class="empty-block-2"></div>');
            }
        });
    });

</script>
