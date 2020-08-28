set_property platform.default_output_type "sd_card" [current_project]
set_property platform.design_intent.embedded "true" [current_project]
set_property platform.design_intent.server_managed "false" [current_project]
set_property platform.design_intent.external_host "false" [current_project]
set_property platform.design_intent.datacenter "false" [current_project]

set_property platform.post_sys_link_tcl_hook ./dynamic_postlink.tcl [current_project]

write_hw_platform -force -include_bit ./vitis_ai_pfm_v1.xsa

validate_hw_platform ./vitis_ai_pfm_v1.xsa