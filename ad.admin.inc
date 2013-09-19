<?php

/**
 * @file
 * Administration page callbacks for the ad module.
 */

/**
 * Form builder. Configure ads.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function ad_admin_settings() {
    $form['c_block_amount'] = array(
        '#type' => 'textfield',
        '#title' => t('Amount of blocks of type C'),
        '#default_value' => variable_get('c_block_amount', 25),
    );
    $form['c_block_template'] = array(
        '#type' => 'textarea',
        '#title' => t('Template for block C'),
        '#default_value' => variable_get('c_block_template', ''),
        '#description' => t('Use [TRAND] for random int'),
    );
    $form['c_fix_block_amount'] = array(
        '#type' => 'textfield',
        '#title' => t('Amount of blocks of type fixed C'),
        '#default_value' => variable_get('c_fix_block_amount', 25),
    );
    $form['c_fix_block_template'] = array(
        '#type' => 'textarea',
        '#title' => t('Template for fixed block C'),
        '#default_value' => variable_get('c_fix_block_template', ''),
        '#description' => t('Use [TRAND] for random int'),
    );
    $form['c_fix1_block_amount'] = array(
        '#type' => 'textfield',
        '#title' => t('Amount of blocks of type fixed C'),
        '#default_value' => variable_get('c_fix1_block_amount', 25),
    );
    $form['c_fix1_block_template'] = array(
        '#type' => 'textarea',
        '#title' => t('Template for fixed block C'),
        '#default_value' => variable_get('c_fix1_block_template', ''),
        '#description' => t('Use [TRAND] for random int'),
    );
    $form['#submit'][] = 'ad_admin_settings_submit';
    return system_settings_form($form);
}

function ad_admin_banner_settings() {
    return 'Some';
}

function ad_admin_banner_add() {
    $form = array();
    $form['machine_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Machine name for a type'),
        '#description' => t('May contain latin characters, numbers and underscores')
    );
    $form['amount'] = array(
        '#type' => 'textfield',
        '#title' => t('Amount of banners of type'),
    );
    $form['template'] = array(
        '#type' => 'textarea',
        '#title' => t('Template for banner'),
        '#description' => t('Use [TRAND] for random int'),
    );
    $form['submit'] = array('#type' => 'submit', '#value' => t('Save'), '#weight' => '1000');
    return $form;
}

function ad_admin_banner_add_submit($form, $form_state) {
    $values = $form_state['values'];
    if (!empty($values['amount']) && !empty($values['template']) && !empty($values['machine_name'])) {
        $name = $values['machine_name'];
        if (preg_match('/^[a-zA-Z0-9_]*$/', $name)) {
            $check = db_select('ad_tpl', 'a')
                    ->fields('a', array('id'))
                    ->condition('id', $name)
                    ->execute()
                    ->fetchCol();
            if ($check)
                form_set_error('machine_name', t('Machine name already exists'));
            else {
                db_insert('ad_tpl')
                        ->fields(array(
                            'id' => $name,
                            'tpl' => $values['template'],
                            'amt' => $values['amount'],
                        ))->execute();
                drupal_set_message(t('Template succefully added'));
                drupal_goto('admin/content/ads');
            }
        }
        else
            form_set_error('machine_name', t('Machine name contains forbidden character(s)'));
    }
}

function ad_admin_blocks_add() {
    $opts = db_select('ad_tpl', 'a')
            ->fields('a', array('id'))
            ->distinct()
            ->execute()
            ->fetchCol();
    if (!$opts && empty($opts)) {
        drupal_set_message(t('Set some templates first'), 'error');
        drupal_goto('admin/content/ads/banner/add');
    } else {
        $options = array();
        foreach ($opts AS $id)
            $options[$id] = $id;
        $form['amount'] = array(
            '#type' => 'textfield',
            '#title' => t('Amount of banners of type'),
            '#default_value' => 0,
            '#description' => t('If set to 0 displays all avalible banners'),
        );
        $form['type'] = array(
            '#type' => 'select',
            '#title' => 'Banner template',
            '#options' => $options,
        );
        $form['name'] = array(
            '#type' => 'textfield',
            '#title' => 'Block name',
        );
        $form['content'] = array(
            '#type' => 'text_format',
            '#title' => 'Additional content',
            '#description' => 'Usage of this content provides by your templates',
        );
        $form['slider'] = array(
            '#type' => 'checkbox',
            '#title' => 'Enable slider',
        );
        $form['submit'] = array('#type' => 'submit', '#value' => t('Save'), '#weight' => '1000');
        return $form;
    }
}

function ad_admin_blocks_add_submit($form, $form_state) {
    $vals = $form_state['values'];
    if (!empty($vals['type'])) {
        $some = db_insert('ad_block')
                ->fields(array(
                    'amt' => $vals['amount'],
                    'type' => $vals['type'],
                    'name' => $vals['name'],
                    'content' => $edit['content']['value'],
                    'slider' => $edit['slider']
                ))
                ->execute();
        drupal_set_message('Block added');
        drupal_goto('admin/structure/block/manage/ad/' . $some . '/configure');
    }
}

function ad_admin_banner_edit_form($form, $form_state, $item) {
    $form['id'] = array(
        '#type' => 'value',
        '#value' => $item->id,
    );
    $form['machine_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Machine name for a type'),
        '#description' => t('May contain latin characters, numbers and underscores'),
        '#disabled' => true,
        '#default_value' => $item->id,
    );
    $form['amount'] = array(
        '#type' => 'textfield',
        '#title' => t('Amount of banners of type'),
        '#default_value' => $item->amt,
    );
    $form['template'] = array(
        '#type' => 'textarea',
        '#title' => t('Template for banner'),
        '#description' => t('Use [TRAND] for random int'),
        '#default_value' => $item->tpl,
    );
    $form['submit'] = array('#type' => 'submit', '#value' => t('Save'), '#weight' => '1000');
    return $form;
}

function ad_admin_banner_edit_form_submit($form, $form_state) {
    $vals = $form_state['values'];
    if ($check = db_select('ad_tpl', 't')
            ->fields('t')
            ->condition('id', $vals['id'])
            ->execute()
            ->fetch()) {
        $updated = db_update('ad_tpl')
                ->fields(array(
                    'amt' => $vals['amount'],
                    'tpl' => $vals['template'],
                ))
                ->condition('id', $vals['id'])
                ->execute();
        if (!$updated)
            drupal_set_message(t('Something gone wrong. Please, connect site administrator'), 'error', false);
        else {
            drupal_set_message(t('Banner data updated'));
        }
    }
}

?>