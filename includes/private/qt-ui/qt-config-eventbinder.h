#ifndef QT_CONFIG_EVENTBINDER_H_
#define QT_CONFIG_EVENTBINDER_H_

void *wx_config_eventbinder(void *hdlg, void (*selectedPageCallback)(void *hdlg, int selectedPage));
void wx_config_destroyeventbinder(void *eventBinder);

#endif /* QT_CONFIG_EVENTBINDER_H_ */
