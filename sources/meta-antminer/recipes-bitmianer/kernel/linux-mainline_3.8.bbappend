do_compile_repend() {
	
}

do_deploy_append() {
	install -d ${DEPLOY_DIR_IMAGE}
	cp -p ${D}/boot/*.dtb ${DEPLOY_DIR_IMAGE}/.
}
