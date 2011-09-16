PROJECT( mtxgauge-demos C )

ADD_EXECUTABLE( gauge-demo gauge-demo.c xmlbase.c )
TARGET_LINK_LIBRARIES( gauge-demo 
	mtxgauge 
	${GTK2_LIBRARIES} 
	gobject-2.0
	xml2
	m
	)
