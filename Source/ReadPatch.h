// read patch by iterating over all attritubes, 
// setting them if they are available and setting to default if not
void OdinAudioProcessor::readPatch(const ValueTree &newState){
  //DBG(newState.toXmlString());

  int minor_version           = newState.getChildWithName("misc")["version_minor"];
  int patch_version           = newState.getChildWithName("misc")["version_patch"];
  int patch_migration_version = newState.getChildWithName("misc")["patch_migration_version"];

  DBG("Read patch from version 2." + std::to_string(minor_version) + "." + std::to_string(patch_version) + ", current version is: 2." + std::to_string(ODIN_MINOR_VERSION) + "." + std::to_string(ODIN_PATCH_VERSION));
  DBG("Read patch migration version " + std::to_string(patch_migration_version) + ", current version is " +
	    std::to_string(ODIN_PATCH_MIGRATION_VERSION));


  const ValueTree &osc_tree = newState.getChildWithName("osc");  
  for (int i = 0; i < m_value_tree_osc.getNumProperties(); ++i) {
    if(osc_tree.hasProperty(m_value_tree_osc.getPropertyName(i))){
        m_value_tree_osc.setProperty(m_value_tree_osc.getPropertyName(i), osc_tree.getProperty(m_value_tree_osc.getPropertyName(i)), nullptr);
        m_value_tree_osc.sendPropertyChangeMessage((m_value_tree_osc.getPropertyName(i)));
    } else {
        DBG("Didn't find property " + m_value_tree_osc.getPropertyName(i).toString().toStdString());
    }
  }
  const ValueTree &fx_tree = newState.getChildWithName("fx");  
  for (int i = 0; i < m_value_tree_fx.getNumProperties(); ++i) {
    if(fx_tree.hasProperty(m_value_tree_fx.getPropertyName(i))){
        m_value_tree_fx.setProperty(m_value_tree_fx.getPropertyName(i), fx_tree.getProperty(m_value_tree_fx.getPropertyName(i)), nullptr);
        m_value_tree_fx.sendPropertyChangeMessage((m_value_tree_fx.getPropertyName(i)));
    } else {
        DBG("Didn't find property " + m_value_tree_fx.getPropertyName(i).toString().toStdString());
    }
  }
  const ValueTree &lfo_tree = newState.getChildWithName("lfo");  
  for (int i = 0; i < m_value_tree_lfo.getNumProperties(); ++i) {
    if(lfo_tree.hasProperty(m_value_tree_lfo.getPropertyName(i))){
        m_value_tree_lfo.setProperty(m_value_tree_lfo.getPropertyName(i), lfo_tree.getProperty(m_value_tree_lfo.getPropertyName(i)), nullptr);
        m_value_tree_lfo.sendPropertyChangeMessage((m_value_tree_lfo.getPropertyName(i)));
    } else {
        DBG("Didn't find property " + m_value_tree_lfo.getPropertyName(i).toString().toStdString());
    }
  }
  const ValueTree &misc_tree = newState.getChildWithName("misc");  
  for (int i = 0; i < m_value_tree_misc.getNumProperties(); ++i) {
    if(misc_tree.hasProperty(m_value_tree_misc.getPropertyName(i))){
        m_value_tree_misc.setProperty(m_value_tree_misc.getPropertyName(i), misc_tree.getProperty(m_value_tree_misc.getPropertyName(i)), nullptr);
        m_value_tree_misc.sendPropertyChangeMessage((m_value_tree_misc.getPropertyName(i)));
    } else {
        DBG("Didn't find property " + m_value_tree_misc.getPropertyName(i).toString().toStdString());
    }
  }
  const ValueTree &mod_tree = newState.getChildWithName("mod");  
  for (int i = 0; i < m_value_tree_mod.getNumProperties(); ++i) {
    if(mod_tree.hasProperty(m_value_tree_mod.getPropertyName(i))){
        m_value_tree_mod.setProperty(m_value_tree_mod.getPropertyName(i), mod_tree.getProperty(m_value_tree_mod.getPropertyName(i)), nullptr);
        m_value_tree_mod.sendPropertyChangeMessage((m_value_tree_mod.getPropertyName(i)));
    } else {
        DBG("Didn't find property " + m_value_tree_mod.getPropertyName(i).toString().toStdString());
    }
  }

  for(int i = 0; i < newState.getNumChildren(); ++i){
      // all children which are an audio param have two properties (name and value)
      if(newState.getChild(i).getNumProperties() == 2){
        DBG(newState.getChild(i).getProperty(newState.getChild(i).getPropertyName(0)).toString());
        DBG(newState.getChild(i).getProperty(newState.getChild(i).getPropertyName(1)).toString());
        String name = newState.getChild(i).getProperty(newState.getChild(i).getPropertyName(0)).toString();
        SETAUDIOFULLRANGESAFE(newState.getChild(i).getProperty(newState.getChild(i).getPropertyName(0)).toString(),newState.getChild(i).getProperty(newState.getChild(i).getPropertyName(1)));
        //DBG("Value on tree is now: is now:" + m_value_tree.getParameterAsValue(name).getValue().toString());
      }
  }
}
