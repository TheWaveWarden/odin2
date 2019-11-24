void OdinAudioProcessor::migratePatch(int p_patch_nr_to_migrate_from) {

    DBG("Migrating patch from patch_version " + std::to_string(p_patch_nr_to_migrate_from));
    // fall through this switch case and append the necessary values along the way:
    switch(p_patch_nr_to_migrate_from){
        case 1:
    	//node.setProperty("fancy_new_nonaudio_param_needed_for_version_2", 0, nullptr);
        //case 2:
        default:
        break;
    }
}