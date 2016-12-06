defmodule PhoenixChat.RoomChannel do
  use Phoenix.Channel
  require Logger

  def join("rooms:lobby", message, socket) do
    connection_count = :ets.info(PhoenixChat.PubSub.Local0)[:size] + 1
    Logger.info "(#{connection_count})"
    {:ok, socket}
  end
end