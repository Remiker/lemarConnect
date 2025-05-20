FROM grafana/grafana:latest
# ENV Variables for cofiguration
## Enable Anonymous Authentication
ENV GF_AUTH_ANONYMOUS_ENABLED=true
# #Activate the ROLE VIEWER as Default
 ENV GF_AUTH_ANONYMOUS_ORG_ROLE=Viewer
# #Disable the AUTH Method
 ENV GF_AUTH_BASIC_ENABLED=false
#  #Disable the login on show
 ENV GF_AUTH_DISABLE_LOGIN_FORM=true
# #Diabled the menuENV GF_AUTH_BASIC_ENABLED=false #Disable the AUTH Method
 ENV GF_AUTH_DISABLE_SIGNOUT_MENU=true
# #Disable the login on show
ENV GF_AUTH_DISABLE_LOGIN_FORM=true
# #Diabled the menu
 ENV GF_AUTH_DISABLE_SIGNOUT_MENU=true

# ## Disable Sanitize
ENV GF_PANELS_DISABLE_SANITIZE_HTML=true

# ## Disable Explore
ENV GF_EXPLORE_ENABLED=false

# # Updates Check
# ENV GF_ANALYTICS_CHECK_FOR_UPDATES=false

## Disable snapshots
# ENV GF_SNAPSHOTS_ENABLED=false
## Paths
ENV GF_PATHS_PROVISIONING="/etc/grafana/provisioning"

ADD ./provisioning /etc/grafana/provisioning
#Add the configuration
COPY ./dashboards /var/lib/grafana/dashboards

ENV GF_DASHBOARDS_DEFAULT_HOME_DASHBOARD_PATH=/var/lib/grafana/dashboards/first_dashboard.json

#Add the Dashboard
EXPOSE 3000


CMD [ "grafana-reporter" ]